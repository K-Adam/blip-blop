#include <fstream>

#include "core/debug.h"
#include "engine/sound.h"
#include "engine/sound_bank.h"
#include "core/asset_path.h"
#include <json.h>

using json = nlohmann::json;

void SoundBank::reload() {
	if (!filename_.empty()) {
		loadSFX(filename_.c_str());
	}
}

bool SoundBank::loadSFX(const char* file_name) {
	auto dir = asset_path_prefix("sfx", file_name);
	std::ifstream input(dir + ".json");
	if (!input.good()) {
		debug << "SoundBank::loadSFX() -> Impossible d'ouvrir le fichier " << dir + ".json"
			  << "\n";
		return false;
	}

	json data = json::parse(input);

	size_t nb_snd = data["items"].size();

	if (nb_snd < 1) {
		debug << "SoundBank::loadSFX() -> Fichier " << file_name << " corrompu\n";
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

	filename_ = file_name;

	return true;
}
