/******************************************************************
 *
 *
 *		------------------
 *		  PictureBank.cpp
 *		------------------
 *
 *		Classe PictureBank
 *
 *		Représente un tableau/une banque d'images.
 *
 *
 *		Prosper / LOADED -   V 0.2 - 13 Juillet 2000
 *
 *
 *
 ******************************************************************/

#include "picture_bank.h"

#include <fstream>

#include <stdio.h>
#include <string.h>
#include "ben_debug.h"
#include "dd_gfx.h"
#include "json.h"
#include "string.h"
#include <SDL2/SDL_image.h>

using json = nlohmann::json;

PictureBank::PictureBank() {}

bool PictureBank::loadGFX(const char* file) {
    auto dir = asset_path_prefix("gfx", file);

    std::ifstream input(dir + ".json");
    if (!input.good()) {
        debug << "PictureBank::loadGFX() - File not found " << dir + ".json" << "\n";
        return false;
    }

    json data = json::parse(input);

    size_t nb_pic = data["items"].size();
    tab_.resize(nb_pic);

    for (size_t i = 0; i < nb_pic; i++) {
        auto item = data["items"][i];

        int xspot = item["xSpot"];
        int yspot = item["ySpot"];

        std::string item_path = item["path"];
        std::string image_path = asset_path_prefix("gfx", item_path.c_str());
        SDL_Surface* surf = IMG_Load(image_path.c_str());

        if (surf == NULL) {
            debug << "Can not load image " << image_path << "\n";
            return false;
        }

        tab_[i] = std::make_unique<Picture>();

        tab_[i]->SetSpot(xspot, yspot);
        tab_[i]->SetSurface(new SDL::Surface(surf));
    }

    filename_ = file;
    return true;
}

bool PictureBank::restoreAll() {
    // TODO: REMOVE
    if (filename_.empty()) {
        return true;
    }

    assert(false);
    
    return false;
}
