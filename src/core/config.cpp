#include "engine/graphics.h"
#include "core/debug.h"
#include "core/config.h"
#include "engine/input.h"
#include "engine/control_alias.h"
#include "engine/key_translator.h"
#include <json.h>

#include <filesystem>
#include <set>
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

bool fullscreen = false;

static const char* ALIAS_NAMES[NB_ACTIONS] = {"UP", "DOWN", "LEFT", "RIGHT", "FIRE", "JUMP", "SUPER", "NEXT_WEAPON", "PREV_WEAPON"};
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

	std::set<int> saved;
	for (auto& [name, scheme] : SCHEMES) {
		json keys = data.value(name, json());
		for (int i = 0; i < NB_ACTIONS; i++) {
			if (!keys.is_object() || !keys.contains(ALIAS_NAMES[i])) continue;
			const json& v = keys[ALIAS_NAMES[i]];
			// Older saves store the raw codes
			unsigned int code = v.is_number_unsigned() ? v.get<unsigned int>() : v.is_string() ? codeFromName(v.get<std::string>()) : UNKNOWN_CODE;
			if (code != UNKNOWN_CODE) {
				in.setAlias(scheme + i, code);
				saved.insert(scheme + i);
			}
		}
	}

	// Actions added after a save keep their default only where it does not steal a saved binding
	for (auto& [name, scheme] : SCHEMES) {
		for (int i = 0; i < NB_ACTIONS; i++) {
			if (saved.count(scheme + i)) continue;
			for (int other : saved) {
				if (in.getAlias(other) == in.getAlias(scheme + i)) in.setAlias(scheme + i, 0);
			}
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

	// Binary keeps LF on Windows, so saving does not rewrite the tracked file
	std::ofstream out(cfg_file, std::ios::binary);
	out << data.dump(2) << '\n';
}

void set_default_config(bool reset_lang) {
	if (reset_lang) lang_type = LANG_UK;

	// The usual keyboard platformer layout, keeping WASD and the mouse free for a second player
	const unsigned int keyboard[NB_ACTIONS] = {DIK_UP, DIK_DOWN, DIK_LEFT, DIK_RIGHT, DIK_X, DIK_Z, DIK_C, DIK_V, DIK_LSHIFT};
	// The mouse aims, so W is free to jump
	const unsigned int mouse[NB_ACTIONS] = {
		0, DIK_S, DIK_A, DIK_D, MOUSE_CODE(SDL_BUTTON_LEFT), DIK_W, MOUSE_CODE(SDL_BUTTON_RIGHT), MOUSE_CODE(MOUSE_WHEEL_DOWN), MOUSE_CODE(MOUSE_WHEEL_UP)};
	// Pad buttons are stored for the first pad, the joining pad replaces the slot.
	// The thumbs stay on the sticks to move and aim, so the actions go on the triggers and a shoulder
	const unsigned int pad[NB_ACTIONS] = {PAD_CODE(0, JOY_UP),
										  PAD_CODE(0, JOY_DOWN),
										  PAD_CODE(0, JOY_LEFT),
										  PAD_CODE(0, JOY_RIGHT),
										  PAD_CODE(0, PAD_RT),
										  PAD_CODE(0, PAD_LT),
										  PAD_CODE(0, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER),
										  PAD_CODE(0, SDL_CONTROLLER_BUTTON_Y),
										  PAD_CODE(0, SDL_CONTROLLER_BUTTON_LEFTSHOULDER)};
	for (int i = 0; i < NB_ACTIONS; i++) {
		in.setAlias(SCHEME_KEYBOARD + i, keyboard[i]);
		in.setAlias(SCHEME_MOUSE + i, mouse[i]);
		in.setAlias(SCHEME_PAD + i, pad[i]);
	}
}
