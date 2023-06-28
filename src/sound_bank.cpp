/******************************************************************
 *
 *
 *		------------------
 *		  SoundBank.cpp
 *		------------------
 *
 *		Classe SoundBank
 *
 *		Représente un tableau/une banque de sons (Sound).
 *
 *
 *		Prosper / LOADED -   V 0.1 - 16 Juillet 2000
 *
 *
 *
 ******************************************************************/

//-----------------------------------------------------------------------------
//		Headers
//-----------------------------------------------------------------------------

#include <malloc.h>
#include <string.h>
#include <fstream>

#include "ben_debug.h"
#include "sound.h"
#include "sound_bank.h"
#include "string.h"
#include "json.h"

using json = nlohmann::json;

//-----------------------------------------------------------------------------
//		Méthodes
//-----------------------------------------------------------------------------

void SoundBank::reload() {
    if (!filename_.empty()) {
        loadSFX(filename_.c_str());
    }
}

bool SoundBank::loadSFX(const char* nom_fic) {
    auto dir = asset_path_prefix("sfx", nom_fic);
    std::ifstream input(dir + ".json");
    if (!input.good()) {
        debug << "SoundBank::loadSFX() -> Impossible d'ouvrir le fichier " << dir + ".json" << "\n";
        return false;
    }

    json data = json::parse(input);

    size_t nb_snd = data["items"].size();

    if (nb_snd < 1) {
        debug << "SoundBank::loadSFX() -> Fichier " << nom_fic << " corrompu\n";
        return false;
    }
    tab_.resize(nb_snd);

    for (size_t i = 0; i < nb_snd; i++) {
        auto item = data["items"][i];

        std::string item_path = data["items"][i];
        auto path = asset_path_prefix("sfx", item_path.c_str());

        tab_[i] = std::make_unique<Sound>();
        tab_[i]->load(path.c_str());
    }

    filename_ = nom_fic;

    return true;
}
