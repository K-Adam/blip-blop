#include "engine/picture_bank.h"

#include <algorithm>
#include <charconv>
#include <fstream>
#include <iterator>
#include <tuple>
#include <vector>

#include "core/debug.h"
#include "engine/graphics.h"
#include <json.h>
#include "core/asset_path.h"
#include <SDL2/SDL_image.h>

using json = nlohmann::json;

// Builds a frame stored as a mirror or right angle turn of another one, so the duplicate pixels are not shipped
static SDL_Surface* derive(SDL_Surface* src, SDL_Rect r, const std::string& op) {
	static const char* const OPS[] = {"copy", "flip_h", "flip_v", "rot180", "rot90ccw", "rot90cw", "transpose", "transverse"};
	int k = static_cast<int>(std::find(std::begin(OPS), std::end(OPS), op) - std::begin(OPS));
	if (k == static_cast<int>(std::size(OPS))) return nullptr;
	int w = r.w, h = r.h;
	bool turned = k >= 4;
	SDL_Surface* dst = SDL_CreateRGBSurfaceWithFormat(0, turned ? h : w, turned ? w : h, 32, SDL_PIXELFORMAT_ARGB8888);
	if (!dst) return nullptr;
	for (int y = 0; y < dst->h; y++) {
		Uint32* row = reinterpret_cast<Uint32*>(static_cast<Uint8*>(dst->pixels) + y * dst->pitch);
		for (int x = 0; x < dst->w; x++) {
			int sx = x, sy = y;
			switch (k) {
				case 1:
					sx = w - 1 - x;
					break;
				case 2:
					sy = h - 1 - y;
					break;
				case 3:
					sx = w - 1 - x, sy = h - 1 - y;
					break;
				case 4:
					sx = w - 1 - y, sy = x;
					break;
				case 5:
					sx = y, sy = h - 1 - x;
					break;
				case 6:
					sx = y, sy = x;
					break;
				case 7:
					sx = w - 1 - y, sy = h - 1 - x;
					break;
			}
			row[x] = (reinterpret_cast<Uint32*>(static_cast<Uint8*>(src->pixels) + (r.y + sy) * src->pitch))[r.x + sx];
		}
	}
	return dst;
}

static SDL_Surface* loadImage(const std::string& path) {
	SDL_Surface* loaded = IMG_Load(asset_path_prefix("gfx", path.c_str()).c_str());
	SDL_Surface* image = loaded ? SDL_ConvertSurfaceFormat(loaded, SDL_PIXELFORMAT_ARGB8888, 0) : nullptr;
	SDL_FreeSurface(loaded);
	return image;
}

// Transparent pixels are stored black, filtering would mix that into the edges as a dark halo.
// They take the colour of their opaque neighbours instead, still fully transparent
static void bleedEdges(SDL_Surface* s) {
	std::vector<Uint32> src(static_cast<Uint32*>(s->pixels), static_cast<Uint32*>(s->pixels) + s->w * s->h);
	Uint32* dst = static_cast<Uint32*>(s->pixels);
	for (int y = 0; y < s->h; y++) {
		for (int x = 0; x < s->w; x++) {
			if (src[y * s->w + x] >> 24) continue;
			int r = 0, g = 0, b = 0, n = 0;
			for (int ny = std::max(y - 1, 0); ny <= std::min(y + 1, s->h - 1); ny++) {
				for (int nx = std::max(x - 1, 0); nx <= std::min(x + 1, s->w - 1); nx++) {
					Uint32 p = src[ny * s->w + nx];
					if (!(p >> 24)) continue;
					r += p >> 16 & 0xFF, g += p >> 8 & 0xFF, b += p & 0xFF, n++;
				}
			}
			if (n) dst[y * s->w + x] = static_cast<Uint32>(r / n) << 16 | static_cast<Uint32>(g / n) << 8 | static_cast<Uint32>(b / n);
		}
	}
}

static const int ATLAS_MAX = 2048;

bool PictureBank::loadGFX(const char* file) {
	auto dir = asset_path_prefix("gfx", file);

	std::ifstream input(dir + ".json");
	if (!input.good()) {
		debug << "PictureBank::loadGFX() - File not found " << dir + ".json"
			  << "\n";
		return false;
	}

	json data = json::parse(input);
	struct Frame {
		int image;	// Into images, later the texture of the same index
		SDL_Rect r;
		int xs, ys;
	};
	std::vector<SDL_Surface*> images;
	std::vector<Frame> frames;
	std::map<std::string, std::pair<int, int>, std::less<>> sheets;
	auto fail = [&](const std::string& why) {
		debug << "Can not load " << file << ": " << why << "\n";
		for (SDL_Surface* s : images) SDL_FreeSurface(s);
		return false;
	};

	for (const json& sheet : data["sheets"]) {
		std::string name = sheet["name"];
		int sheet_image = -1;
		if (sheet.contains("path")) {
			images.push_back(loadImage(sheet["path"]));
			if (!images.back()) return fail(sheet["path"]);
			sheet_image = static_cast<int>(images.size()) - 1;
		}
		int first = static_cast<int>(frames.size());
		for (const json& f : sheet["frames"]) {
			Frame fr{sheet_image, {0, 0, 0, 0}, f["xSpot"], f["ySpot"]};
			if (f.contains("from")) {
				// Sources always come earlier, in this sheet or a previous one
				std::string from = f["from"];
				int src_first = first, src_count = static_cast<int>(frames.size()) - first;
				if (from != name) {
					auto it = sheets.find(from);
					if (it == sheets.end()) return fail("bad source " + from);
					std::tie(src_first, src_count) = it->second;
				}
				int k = f["frame"];
				if (k < 0 || k >= src_count) return fail("bad source " + from);
				const Frame& src = frames[src_first + k];
				images.push_back(derive(images[src.image], src.r, f["op"]));
				if (!images.back()) return fail("bad op in " + name);
				fr.image = static_cast<int>(images.size()) - 1;
				fr.r = {0, 0, images.back()->w, images.back()->h};
			} else if (f.contains("path")) {
				images.push_back(loadImage(f["path"]));
				if (!images.back()) return fail(f["path"]);
				fr.image = static_cast<int>(images.size()) - 1;
				fr.r = {0, 0, images.back()->w, images.back()->h};
			} else {
				if (sheet_image < 0) return fail(name + " has no image");
				fr.r = {f["x"].get<int>(), f["y"].get<int>(), f["w"].get<int>(), f["h"].get<int>()};
				// derive() reads raw pixels of the frame
				const SDL_Surface* img = images[sheet_image];
				if (fr.r.x < 0 || fr.r.y < 0 || fr.r.w <= 0 || fr.r.h <= 0 || fr.r.x + fr.r.w > img->w || fr.r.y + fr.r.h > img->h)
					return fail("frame outside " + name);
			}
			frames.push_back(fr);
		}
		sheets[name] = {first, static_cast<int>(frames.size()) - first};
	}

	// Repacked with a transparent border, so filtered sampling of a rotated or shrunk frame never reaches its neighbours
	size_t sources = images.size();
	for (size_t n = 0; n < frames.size();) {
		int w = 0, x = 0, y = 0, rowh = 0;
		size_t end = n;
		std::vector<SDL_Point> pos;
		for (; end < frames.size(); end++) {
			int w2 = frames[end].r.w + 2, h2 = frames[end].r.h + 2;
			if (x && x + w2 > ATLAS_MAX) x = 0, y += rowh, rowh = 0;
			if (y && y + h2 > ATLAS_MAX) break;
			pos.push_back({x + 1, y + 1});
			x += w2;
			w = std::max(w, x);
			rowh = std::max(rowh, h2);
		}
		SDL_Surface* atlas = SDL_CreateRGBSurfaceWithFormat(0, w, y + rowh, 32, SDL_PIXELFORMAT_ARGB8888);
		if (!atlas) return fail("no memory");
		images.push_back(atlas);
		for (size_t m = n; m < end; m++) {
			Frame& fr = frames[m];
			SDL_Surface* part = images[fr.image];
			SDL_SetSurfaceBlendMode(part, SDL_BLENDMODE_NONE);
			SDL_Rect d{pos[m - n].x, pos[m - n].y, fr.r.w, fr.r.h};
			SDL_BlitSurface(part, &fr.r, atlas, &d);
			fr.image = static_cast<int>(images.size()) - 1;
			fr.r = d;
		}
		bleedEdges(atlas);
		n = end;
	}
	for (size_t i = 0; i < sources; i++) {
		SDL_FreeSurface(images[i]);
		images[i] = nullptr;
	}
	textures_.clear();
	for (SDL_Surface* image : images) textures_.push_back(image ? std::make_unique<SDL::Surface>(image) : nullptr);
	frames_.clear();
	frames_.reserve(frames.size());
	for (const Frame& f : frames) frames_.emplace_back(textures_[f.image].get(), f.r, f.xs, f.ys);
	sheets_ = std::move(sheets);
	file_ = file;
	return true;
}

Flipbook PictureBank::anim(std::string_view name) const {
	auto it = sheets_.find(name);
	if (it == sheets_.end()) {
		debug << "Bug: no animation " << name << " in " << file_ << std::endl;
		return Flipbook(frames_.data(), 0, name);
	}
	return Flipbook(frames_.data() + it->second.first, it->second.second, it->first);
}

const Picture* PictureBank::frame(std::string_view ref) const {
	size_t colon = ref.find(':');
	int n = 0;
	if (colon != std::string_view::npos) std::from_chars(ref.data() + colon + 1, ref.data() + ref.size(), n);
	Flipbook f = anim(ref.substr(0, colon));
	return n >= 0 && n < f.size() ? f[n] : nullptr;
}
