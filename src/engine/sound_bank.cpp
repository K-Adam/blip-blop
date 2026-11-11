#include <fstream>

#include "core/asset_path.h"
#include "engine/sound.h"
#include "core/debug.h"
#include "engine/sound_bank.h"
#include <json.h>

using json = nlohmann::json;

bool SoundBank::loadSFX(const char* file_name) {
	if (!audio.sound()) return true;

	auto dir = asset_path_prefix("sfx", file_name);
	std::ifstream input(dir + ".json");
	if (!input.good()) {
		debug << "SoundBank::loadSFX() -> Cannot open " << dir + ".json"
			  << "\n";
		return false;
	}

	json data = json::parse(input);

	size_t nb_snd = data["items"].size();

	if (nb_snd < 1) {
		debug << "SoundBank::loadSFX() -> File " << file_name << " corrupted!\n";
		return false;
	}
	tab_.clear();
	tab_.resize(nb_snd);

	for (size_t i = 0; i < nb_snd; i++) {
		// A missing sound stays silent instead of failing the whole bank
		tab_[i].load(asset_path_prefix("sfx", data["items"][i].get<std::string>().c_str()).c_str());
	}

	return true;
}

void SoundBank::play(int n, int flags) {
	if (!audio.sound()) return;

	if (static_cast<size_t>(n) >= tab_.size()) {
		debug << "Trying to play missing sound " << n << "\n";
		return;
	}

	tab_[n].play(flags);
}

void SoundBank::stop(int n) {
	if (!audio.sound()) return;

	tab_[n].stop();
}
