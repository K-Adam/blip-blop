#include "engine/dd_gfx.h"
#include "core/debug.h"
#include "core/config.h"
#include "engine/input.h"
#include "engine/control_alias.h"
#include "engine/fmod.h"
#include "core/globals.h"
#include <json.h>

#include <filesystem>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

using json = nlohmann::json;

bool vSyncOn = true;

int lang_type = LANG_UK;

bool music_on = true;
bool sound_on = true;

bool cheat_on = false;

HiScores hi_scores;

bool fullscreen = false;  // THIS IS UGLY AS FUCK. WAY TOO MANY GLOBALS

static const char* ALIAS_NAMES[7] = {"UP", "DOWN", "LEFT", "RIGHT", "FIRE", "JUMP", "SUPER"};

// IndexedDB starts empty on a first visit, the preloaded files are the defaults
void restore_default_saves() {
#ifdef __EMSCRIPTEN__
	for (const char* f : {CONFIG_FILE, HISCORES_FILE}) {
		std::error_code ec;
		std::filesystem::copy_file(std::string("config_defaults/") + std::filesystem::path(f).filename().string(), f,
								   std::filesystem::copy_options::skip_existing, ec);
	}
#endif
}

void save_settings() {
	if (!hi_scores.save(HISCORES_FILE)) debug << "Cannot save hi-scores\n";
	save_BB3_config(CONFIG_FILE);
#ifdef __EMSCRIPTEN__
	EM_ASM(FS.syncfs(false, function(err){}););
#endif
}

void load_BB3_config(const char* cfg_file) {
	set_default_config(true);

	std::ifstream input(cfg_file);
	json data = json::parse(input, nullptr, false);
	if (!data.is_object()) return;

	auto flag = [&](const char* key, bool def) { return data.contains(key) && data[key].is_boolean() ? data[key].get<bool>() : def; };
	vSyncOn = flag("vsync", vSyncOn);
	fullscreen = flag("fullscreen", fullscreen);

	for (int p = 0; p < 2; p++) {
		json keys = data.value(p == 0 ? "player1" : "player2", json());
		for (int i = 0; i < 7; i++) {
			if (keys.is_object() && keys.contains(ALIAS_NAMES[i]) && keys[ALIAS_NAMES[i]].is_number_integer()) {
				in.setAlias(p * 10 + i, keys[ALIAS_NAMES[i]].get<unsigned int>());
			}
		}
	}
}

void save_BB3_config(const char* cfg_file) {
	json data;

	data["vsync"] = vSyncOn;
	data["fullscreen"] = fullscreen;

	for (int p = 0; p < 2; p++) {
		json& keys = data[p == 0 ? "player1" : "player2"];
		for (int i = 0; i < 7; i++) keys[ALIAS_NAMES[i]] = in.getAlias(p * 10 + i);
	}

	std::ofstream out(cfg_file);
	out << data.dump(2);
}

void set_default_config(bool reset_lang) {
	if (reset_lang) lang_type = LANG_UK;

	in.setAlias(ALIAS_P1_UP, DIK_UP);
	in.setAlias(ALIAS_P1_DOWN, DIK_DOWN);
	in.setAlias(ALIAS_P1_LEFT, DIK_LEFT);
	in.setAlias(ALIAS_P1_RIGHT, DIK_RIGHT);
	in.setAlias(ALIAS_P1_FIRE, DIK_LSHIFT);
	in.setAlias(ALIAS_P1_JUMP, DIK_SPACE);
	in.setAlias(ALIAS_P1_SUPER, DIK_BACKSPACE);

	in.setAlias(ALIAS_P2_UP, DIK_W);
	in.setAlias(ALIAS_P2_DOWN, DIK_S);
	in.setAlias(ALIAS_P2_LEFT, DIK_A);
	in.setAlias(ALIAS_P2_RIGHT, DIK_D);
	in.setAlias(ALIAS_P2_FIRE, DIK_TAB);
	in.setAlias(ALIAS_P2_JUMP, DIK_Q);
	in.setAlias(ALIAS_P2_SUPER, DIK_F);
}
