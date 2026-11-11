#include "core/debug.h"
#include "core/config.h"
#include "engine/input.h"
#include "engine/control_alias.h"
#include "engine/key_translator.h"
#include <json.h>

#include <fstream>
#include <set>
#include <utility>

using json = nlohmann::json;

static const char* ALIAS_NAMES[NB_ACTIONS] = {"UP", "DOWN", "LEFT", "RIGHT", "FIRE", "JUMP", "SUPER", "NEXT_WEAPON", "PREV_WEAPON"};
static const std::pair<const char*, int> SCHEMES[] = {{"keyboard", SCHEME_KEYBOARD}, {"mouse", SCHEME_MOUSE}, {"controller", SCHEME_PAD}};

void Settings::load(const char* file, Input& input) {
	setDefaults(input, true);

	std::ifstream stream(file);
	json data = json::parse(stream, nullptr, false);
	if (!data.is_object()) return;

	auto flag = [&](const char* key, bool def) { return data.contains(key) && data[key].is_boolean() ? data[key].get<bool>() : def; };
	vsync = flag("vsync", vsync);
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
				input.setAlias(scheme + i, code);
				saved.insert(scheme + i);
			}
		}
	}

	// Actions added after a save keep their default only where it does not steal a saved binding
	for (auto& [name, scheme] : SCHEMES) {
		for (int i = 0; i < NB_ACTIONS; i++) {
			if (saved.count(scheme + i)) continue;
			for (int other : saved) {
				if (input.getAlias(other) == input.getAlias(scheme + i)) input.setAlias(scheme + i, 0);
			}
		}
	}
}

void Settings::save(const char* file, const Input& input) const {
	json data;

	data["vsync"] = vsync;
	data["fullscreen"] = fullscreen;

	for (auto& [name, scheme] : SCHEMES) {
		json& keys = data[name];
		for (int i = 0; i < NB_ACTIONS; i++) keys[ALIAS_NAMES[i]] = codeName(input.getAlias(scheme + i));
	}

	// Binary keeps LF on Windows, so saving does not rewrite the tracked file
	std::ofstream out(file, std::ios::binary);
	out << data.dump(2) << '\n';
}

void Settings::setDefaults(Input& input, bool reset_lang) {
	if (reset_lang) lang = LANG_UK;

	// The usual keyboard platformer layout, keeping WASD and the mouse free for a second player
	const unsigned int keyboard[NB_ACTIONS] = {SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_x, SDLK_z, SDLK_c, SDLK_v, SDLK_LSHIFT};
	// The mouse aims, so W is free to jump
	const unsigned int mouse[NB_ACTIONS] = {
		0, SDLK_s, SDLK_a, SDLK_d, MOUSE_CODE(SDL_BUTTON_LEFT), SDLK_w, MOUSE_CODE(SDL_BUTTON_RIGHT), MOUSE_CODE(MOUSE_WHEEL_DOWN), MOUSE_CODE(MOUSE_WHEEL_UP)};
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
		input.setAlias(SCHEME_KEYBOARD + i, keyboard[i]);
		input.setAlias(SCHEME_MOUSE + i, mouse[i]);
		input.setAlias(SCHEME_PAD + i, pad[i]);
	}
}
