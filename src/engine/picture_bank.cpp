#include "engine/picture_bank.h"

#include <algorithm>
#include <fstream>
#include <iterator>
#include <vector>

#include "core/debug.h"
#include "engine/graphics.h"
#include <json.h>
#include "core/asset_path.h"
#include <SDL2/SDL_image.h>

using json = nlohmann::json;

PictureBank::PictureBank() {}

// Builds an item stored as a mirror or right angle turn of another one, so the duplicate PNG is not shipped
static SDL_Surface* derive(SDL_Surface* src, const std::string& op) {
	static const char* const OPS[] = {"copy", "flip_h", "flip_v", "rot180", "rot90ccw", "rot90cw", "transpose", "transverse"};
	int k = int(std::find(std::begin(OPS), std::end(OPS), op) - std::begin(OPS));
	if (k == int(std::size(OPS))) return nullptr;
	int w = src->w, h = src->h;
	bool turned = k >= 4;
	SDL_Surface* dst = SDL_CreateRGBSurfaceWithFormat(0, turned ? h : w, turned ? w : h, 32, SDL_PIXELFORMAT_ARGB8888);
	if (!dst) return nullptr;
	for (int y = 0; y < dst->h; y++) {
		Uint32* row = (Uint32*)((Uint8*)dst->pixels + y * dst->pitch);
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
			row[x] = ((Uint32*)((Uint8*)src->pixels + sy * src->pitch))[sx];
		}
	}
	return dst;
}

bool PictureBank::loadGFX(const char* file) {
	auto dir = asset_path_prefix("gfx", file);

	std::ifstream input(dir + ".json");
	if (!input.good()) {
		debug << "PictureBank::loadGFX() - File not found " << dir + ".json"
			  << "\n";
		return false;
	}

	json data = json::parse(input);
	const json& items = data["items"];
	size_t nb_pic = items.size();

	std::vector<SDL_Surface*> images(nb_pic, nullptr);
	bool ok = true;
	for (size_t i = 0; i < nb_pic && ok; i++) {
		if (items[i].contains("path")) {
			std::string image_path = asset_path_prefix("gfx", items[i]["path"].get<std::string>().c_str());
			SDL_Surface* loaded = IMG_Load(image_path.c_str());
			images[i] = loaded ? SDL_ConvertSurfaceFormat(loaded, SDL_PIXELFORMAT_ARGB8888, 0) : nullptr;
			SDL_FreeSurface(loaded);
		} else {
			// Derived items only point at earlier ones
			size_t from = items[i].value("from", i);
			images[i] = from < i ? derive(images[from], items[i].value("op", "")) : nullptr;
		}
		if (!images[i]) {
			debug << "Can not load item " << i << " of " << file << "\n";
			ok = false;
		}
	}
	if (!ok) {
		for (SDL_Surface* s : images) SDL_FreeSurface(s);
		return false;
	}

	tab_.resize(nb_pic);
	for (size_t i = 0; i < nb_pic; i++) {
		tab_[i] = std::make_unique<Picture>();
		tab_[i]->SetSpot(items[i]["xSpot"], items[i]["ySpot"]);
		tab_[i]->SetSurface(new SDL::Surface(images[i]));
	}
	return true;
}
