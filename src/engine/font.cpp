#include <fstream>

#include "engine/graphics.h"

#include "core/debug.h"
#include "engine/font.h"
#include <json.h>
#include "core/asset_path.h"

#include <SDL2/SDL_image.h>

using json = nlohmann::json;

bool Font::load(const char* fic) {
	auto font_path = asset_path_prefix("lft", fic);
	std::ifstream input(font_path + ".json");
	if (!input.good()) {
		debug << "Font::load->Ne peut pas ouvrir " << font_path + ".json"
			  << "\n";
		return false;
	}

	json data = json::parse(input);

	pictab_.clear();

	h = data["h"];
	spc = data["spc"];

	pictab_.resize(256);

	for (int i = 1; i < 256; i++) {
		auto key = std::to_string(i);
		if (data["items"].find(key) == data["items"].end()) {
			continue;
		}

		auto item = data["items"][key];

		std::string item_path = item["path"];
		std::string image_path = asset_path_prefix("lft", item_path.c_str());
		SDL_Surface* surf = IMG_Load(image_path.c_str());

		if (surf == NULL) {
			debug << "Can not load image " << image_path << "\n";
			return false;
		}

		pictab_[i] = std::make_unique<Picture>();
		pictab_[i]->SetSurface(new SDL::Surface(surf));
		pictab_[i]->SetSpot(0, 0);
	}

	return true;
}

void Font::print(SDL::Surface* surf, int x, int y, std::string_view txt) {
	int curx = x;

	for (unsigned char c : txt) {
		if (c == ' ') {
			curx += spc;
		} else if (pictab_[c] != NULL) {
			pictab_[c]->BlitTo(surf, curx, y);
			curx += pictab_[c]->xSize();
		}
	}
}

void Font::printM(SDL::Surface* surf, int x, int y, std::string_view txt, int ym) {
	int curx = x;
	int cury = y;

	for (unsigned char c : txt) {
		if (c == ' ') {
			curx += spc;
		} else if (pictab_[c] != NULL) {
			int nx = curx + pictab_[c]->xSize();

			if (nx >= ym) {
				cury += h;
				pictab_[c]->BlitTo(surf, x, cury);
				curx = x + pictab_[c]->xSize();
			} else {
				pictab_[c]->BlitTo(surf, curx, cury);
				curx = nx;
			}
		}
	}
}

void Font::printR(SDL::Surface* surf, int x, int y, std::string_view txt) { print(surf, x - width(txt), y, txt); }

void Font::printC(SDL::Surface* surf, int xtaille, int y, std::string_view txt) { print(surf, xtaille - (width(txt) >> 1), y, txt); }

void Font::printMW(SDL::Surface* surf, int x, int y, std::string_view txt, int ym) {
	int curx = x;
	int cury = y;
	size_t pos = 0;

	while ((pos = txt.find_first_not_of(' ', pos)) != std::string_view::npos) {
		size_t end = txt.find(' ', pos);
		std::string_view word = txt.substr(pos, end - pos);
		pos = end;
		int nx = width(word);

		if (curx + nx > ym) {
			cury += h;
			print(surf, x, cury, word);
			curx = x + nx;
		} else {
			print(surf, curx, cury, word);
			curx += nx;
		}

		curx += spc;
	}
}

int Font::width(std::string_view txt) {
	int l = 0;

	for (unsigned char c : txt) {
		if (c == ' ')
			l += spc;
		else if (pictab_[c] != NULL)
			l += pictab_[c]->xSize();
	}

	return l;
}
