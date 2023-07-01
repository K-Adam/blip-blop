/******************************************************************
 *
 *
 *		----------------
 *		  Fonte.h
 *		----------------
 *
 *		Classe Fonte
 *
 *		Affiche du joli texte
 *
 *
 *		Prosper / LOADED -   V 0.2
 *
 *
 *
 ******************************************************************/

//-----------------------------------------------------------------------------
//		Headers
//-----------------------------------------------------------------------------

#include <fstream>

#include "graphics.h"
#include <string.h>
#include <fstream>
#include <fcntl.h>
#include <stdio.h>

#include "ben_debug.h"
#include "fonte.h"
#include <json.h>
#include "string.h"

#include <SDL2/SDL_image.h>

#ifdef _MSC_VER
// TODO
#pragma warning(disable : 4996)
#endif

using json = nlohmann::json;

//-----------------------------------------------------------------------------

bool Fonte::load(const char* fic) {
	auto font_path = asset_path_prefix("lft", fic);
	std::ifstream input(font_path + ".json");
	if (!input.good()) {
		debug << "Fonte::load->Ne peut pas ouvrir " << font_path + ".json"
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

	filename_ = fic;

	return true;
}

//-----------------------------------------------------------------------------

void Fonte::print(SDL::Surface* surf, int x, int y, const char* txt) {
	if (txt == NULL) return;

	int curx;  // X courant
	int c;

	curx = x;

	for (unsigned int i = 0; i < strlen(txt); i++) {
		c = (unsigned char)txt[i];
		if (c == ' ') {
			curx += spc;
		} else if (pictab_[c] != NULL) {
			pictab_[c]->BlitTo(surf, curx, y);
			curx += pictab_[c]->xSize();
		}
	}
}

//-----------------------------------------------------------------------------

void Fonte::printM(SDL::Surface* surf, int x, int y, const char* txt, int ym) {
	if (txt == NULL) return;

	int curx;  // X courant
	int cury;  // Y courant
	int nx;	   // Next x (pour ne pas le calculer 2 fois)
	unsigned int c;

	curx = x;
	cury = y;

	for (unsigned int i = 0; i < strlen(txt); i++) {
		c = (unsigned char)txt[i];

		if (c == ' ') {
			curx += spc;
		} else if (pictab_[c] != NULL) {
			// Calcul du X suivant
			nx = curx + pictab_[c]->xSize();

			// Ligne suivante ?
			if (nx >= ym) {
				cury += h;
				pictab_[c]->BlitTo(surf, x, cury);
				curx = x + pictab_[c]->xSize();
			} else {  // Non, on continue..
				pictab_[c]->BlitTo(surf, curx, cury);
				curx = nx;
			}
		}
	}
}

//-----------------------------------------------------------------------------

void Fonte::printR(SDL::Surface* surf, int x, int y, const char* txt) {
	if (txt == NULL) return;

	int l = 0;	// Longueur en pixels de la chaîne
	int c;

	for (unsigned int i = 0; i < strlen(txt); i++) {
		c = (unsigned char)txt[i];

		if (c == ' ')
			l += spc;
		else if (pictab_[c] != NULL)
			l += pictab_[c]->xSize();
	}

	print(surf, x - l, y, txt);
}

//-----------------------------------------------------------------------------

void Fonte::printC(SDL::Surface* surf, int xtaille, int y, const char* txt) {
	if (txt == NULL) return;

	int l = 0;	// Longueur en pixels de la chaîne
	int c;

	for (unsigned int i = 0; i < strlen(txt); i++) {
		c = (unsigned char)txt[i];

		if (c == ' ')
			l += spc;
		else if (pictab_[c] != NULL)
			l += pictab_[c]->xSize();
	}

	print(surf, xtaille - (l >> 1), y, txt);
}

//-----------------------------------------------------------------------------

void Fonte::printMW(SDL::Surface* surf, int x, int y, const char* srctxt, int ym) {
	static const char delim[] = " ";

	if (srctxt == NULL) return;

	int curx;  // X courant
	int cury;  // Y courant
	int nx;	   // Next x (pour ne pas le calculer 2 fois)

	curx = x;
	cury = y;

	char* txt = new char[strlen(srctxt) + 1];
	char* token;

	strcpy(txt, srctxt);
	token = strtok(txt, delim);

	while (token != NULL) {
		nx = width(token);

		if (curx + nx > ym) {
			cury += h;
			print(surf, x, cury, token);
			curx = x + nx;
		} else {
			print(surf, curx, cury, token);
			curx += nx;
		}

		curx += spc;

		token = strtok(NULL, delim);
	}
}

//-----------------------------------------------------------------------------

int Fonte::width(const char* txt) {
	if (txt == NULL) return 0;

	int l = 0;	// Longueur en pixels de la chaîne
	int c;

	for (unsigned int i = 0; i < strlen(txt); i++) {
		c = (unsigned char)txt[i];

		if (c == ' ')
			l += spc;
		else if (pictab_[c] != NULL)
			l += pictab_[c]->xSize();
	}

	return l;
}

//-----------------------------------------------------------------------------

bool Fonte::restoreAll() {
	if (filename_.empty()) {
		return true;
	}

	assert(false);

	return true;
}
