#include "music_bank.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include "ben_debug.h"
#include "config.h"
#include "fmod__errors.h"
#include "string.h"
#include <json.h>

using json = nlohmann::json;

bool MusicBank::open(const char* file, bool loop) {
	if (!music_on) return true;

	auto dir = asset_path_prefix("mbk", file);
	std::ifstream input(dir + ".json");
	if (!input.good()) {
		debug << "MusicBank::load() -> Impossible d'ouvrir le fichier " << dir + ".json"
			  << "\n";
		return false;
	}

	json data = json::parse(input);

	size_t nb_musics = data["items"].size();
	if (nb_musics < 1) {
		debug << "MusicBank::load() -> Fichier " << file << " corrompu (" << nb_musics << ")\n";
		return false;
	}

	musics_.resize(nb_musics);

	for (size_t i = 0; i < nb_musics; i++) {
		std::string path = data["items"][i];
		musics_[i].reset(new Mp3Music(asset_path_prefix("mbk", path.c_str()), loop));
	}

	return true;
}

void MusicBank::play(int n) {
	if (!music_on) return;

	if (n < 0 || (size_t)n >= musics_.size()) {
		debug << "MusicBank::play() -> Tentative de jouer une musique non "
				 "chargée : "
			  << n << "\n";
		return;
	}

	musics_[n]->Play();
}

void MusicBank::stop(int n) {
	if (!music_on) return;

	if (n < 0 || (size_t)n >= musics_.size()) {
		debug << "MusicBank::stop() -> Tentative de stoper une musique non "
				 "chargée : "
			  << n << "\n";
		return;
	}

	musics_[n]->Stop();
}

void MusicBank::stop() {
	for (int i = 0; (size_t)i < musics_.size(); i++) {
		stop(i);
	}
}

void MusicBank::setVol(int v) {
	if (!music_on) return;

	for (size_t i = 0; i < musics_.size(); i++) {
		musics_[i]->set_volume(v);
	}
}
