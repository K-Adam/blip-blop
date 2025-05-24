#include "engine/dd_gfx.h"
#include "core/debug.h"
#include "core/config.h"
#include "engine/input.h"
#include "engine/control_alias.h"
#include "engine/key_translator.h"
#include "engine/fmod.h"
#include "core/globals.h"
#include <json.h>

#include <filesystem>
#include <utility>

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

static const char* ALIAS_NAMES[NB_ACTIONS] = {"UP", "DOWN", "LEFT", "RIGHT", "FIRE", "JUMP", "SUPER"};
static const std::pair<const char*, int> SCHEMES[] = {{"keyboard", SCHEME_KEYBOARD}, {"mouse", SCHEME_MOUSE}, {"controller", SCHEME_PAD}};

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

	for (auto& [name, scheme] : SCHEMES) {
		json keys = data.value(name, json());
		for (int i = 0; i < NB_ACTIONS; i++) {
			if (!keys.is_object() || !keys.contains(ALIAS_NAMES[i]) || !keys[ALIAS_NAMES[i]].is_string()) continue;
			unsigned int code = codeFromName(keys[ALIAS_NAMES[i]].get<std::string>());
			if (code != UNKNOWN_CODE) in.setAlias(scheme + i, code);
		}
	}
}

void save_BB3_config(const char* cfg_file) {
	json data;

	data["vsync"] = vSyncOn;
	data["fullscreen"] = fullscreen;

	for (auto& [name, scheme] : SCHEMES) {
		json& keys = data[name];
		for (int i = 0; i < NB_ACTIONS; i++) keys[ALIAS_NAMES[i]] = codeName(in.getAlias(scheme + i));
	}

	std::ofstream out(cfg_file);
	out << data.dump(2);
}

void set_default_config(bool reset_lang) {
	if (reset_lang) lang_type = LANG_UK;

	// The usual keyboard platformer layout, keeping WASD and the mouse free for a second player
	const unsigned int keyboard[NB_ACTIONS] = {DIK_UP, DIK_DOWN, DIK_LEFT, DIK_RIGHT, DIK_X, DIK_Z, DIK_C};
	const unsigned int mouse[NB_ACTIONS] = {DIK_W, DIK_S, DIK_A, DIK_D, MOUSE_CODE(SDL_BUTTON_LEFT), DIK_SPACE, MOUSE_CODE(SDL_BUTTON_RIGHT)};
	// Pad buttons are stored for the first pad, the joining pad replaces the slot
	const unsigned int pad[NB_ACTIONS] = {PAD_CODE(0, JOY_UP),
										  PAD_CODE(0, JOY_DOWN),
										  PAD_CODE(0, JOY_LEFT),
										  PAD_CODE(0, JOY_RIGHT),
										  PAD_CODE(0, SDL_CONTROLLER_BUTTON_X),
										  PAD_CODE(0, SDL_CONTROLLER_BUTTON_A),
										  PAD_CODE(0, SDL_CONTROLLER_BUTTON_B)};
	for (int i = 0; i < NB_ACTIONS; i++) {
		in.setAlias(SCHEME_KEYBOARD + i, keyboard[i]);
		in.setAlias(SCHEME_MOUSE + i, mouse[i]);
		in.setAlias(SCHEME_PAD + i, pad[i]);
	}
}
