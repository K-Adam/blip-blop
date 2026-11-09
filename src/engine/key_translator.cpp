#include <charconv>
#include <iterator>
#include <string>

#include "core/config.h"
#include "engine/input.h"
#include "engine/key_translator.h"

static std::string DIK_to_string_us(int n);
static std::string DIK_to_string_fr(int n);

static std::string mouse_to_string(int n, bool fr) {
	switch (n & 0x3FF) {
		case SDL_BUTTON_LEFT:
			return fr ? "SOURIS GAUCHE" : "MOUSE LEFT";
		case SDL_BUTTON_RIGHT:
			return fr ? "SOURIS DROITE" : "MOUSE RIGHT";
		case SDL_BUTTON_MIDDLE:
			return fr ? "SOURIS MILIEU" : "MOUSE MIDDLE";
	}
	return (fr ? "SOURIS " : "MOUSE ") + std::to_string(n & 0x3FF);
}

static std::string pad_to_string(int n, bool fr) {
	int d = n & 0x3FF;
	std::string name;
	switch (d) {
		case JOY_UP:
			name = fr ? "HAUT" : "UP";
			break;
		case JOY_DOWN:
			name = fr ? "BAS" : "DOWN";
			break;
		case JOY_LEFT:
			name = fr ? "GAUCHE" : "LEFT";
			break;
		case JOY_RIGHT:
			name = fr ? "DROITE" : "RIGHT";
			break;
		case PAD_LT:
			name = "LT";
			break;
		case PAD_RT:
			name = "RT";
			break;
		case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
			name = "LB";
			break;
		case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
			name = "RB";
			break;
		default: {
			const char* s = d < SDL_CONTROLLER_BUTTON_MAX ? SDL_GameControllerGetStringForButton(SDL_GameControllerButton(d)) : nullptr;
			name = s ? s : std::to_string(d);
			for (char& c : name) c = char(toupper(c));
		}
	}
	return std::string(fr ? "MANETTE " : "PAD ") + name;
}

std::string DIK_to_string(int n) {
	if (lang_type == LANG_FR) {
		return DIK_to_string_fr(n);
	} else {
		return DIK_to_string_us(n);
	}
}

std::string DIK_to_string_us(int n) {
	if (codeSlot(n) == MOUSE_SLOT) return mouse_to_string(n, false);
	if (codeSlot(n)) return pad_to_string(n, false);

	switch (n) {
#define X(_num, _val) \
	case _num:        \
		return _val;
#include "engine/keys_us.xmacro"
#undef X
		default:
			return "UNDEFINED";
	}
}

std::string DIK_to_string_fr(int n) {
	if (codeSlot(n) == MOUSE_SLOT) return mouse_to_string(n, true);
	if (codeSlot(n)) return pad_to_string(n, true);

	switch (n) {
#define X(_num, _val) \
	case _num:        \
		return _val;
#include "engine/keys_fr.xmacro"
#undef X
		default:
			return "INDEFINIE";
	}
}

static const char* const PAD_EXTRA[] = {"lefttrigger", "righttrigger"};
static const char* const PAD_DIRS[] = {"up", "down", "left", "right"};

std::string codeName(unsigned int code) {
	if (code == 0) return "";
	int b = code & 0x3FF;
	if (codeSlot(code) == MOUSE_SLOT) return "Mouse " + std::to_string(b);
	if (codeSlot(code)) {
		if (b >= JOY_UP && b <= JOY_RIGHT) return std::string("Pad ") + PAD_DIRS[b - JOY_UP];
		if (b == PAD_LT || b == PAD_RT) return std::string("Pad ") + PAD_EXTRA[b - PAD_LT];
		const char* s = b < SDL_CONTROLLER_BUTTON_MAX ? SDL_GameControllerGetStringForButton(SDL_GameControllerButton(b)) : nullptr;
		return s ? std::string("Pad ") + s : std::to_string(code);
	}
	std::string name = SDL_GetKeyName(SDL_Keycode(code));
	// Layout fallback codes read back as another key, those stay numeric
	if (name.empty() || codeFromName(name) != code) return std::to_string(code);
	return name;
}

unsigned int codeFromName(const std::string& name) {
	if (name.empty()) return 0;
	unsigned int v = 0;
	auto [end, err] = std::from_chars(name.data(), name.data() + name.size(), v);
	if (err == std::errc() && end == name.data() + name.size()) return v;
	if (name.rfind("Mouse ", 0) == 0) {
		int b = 0;
		auto [p, ec] = std::from_chars(name.data() + 6, name.data() + name.size(), b);
		return ec == std::errc() && b > 0 && b < MOUSE_BUTTONS ? MOUSE_CODE(b) : UNKNOWN_CODE;
	}
	if (name.rfind("Pad ", 0) == 0) {
		std::string b = name.substr(4);
		for (int i = 0; i < (int)std::size(PAD_DIRS); i++) {
			if (b == PAD_DIRS[i]) return PAD_CODE(0, JOY_UP + i);
		}
		for (int i = 0; i < (int)std::size(PAD_EXTRA); i++) {
			if (b == PAD_EXTRA[i]) return PAD_CODE(0, PAD_LT + i);
		}
		SDL_GameControllerButton btn = SDL_GameControllerGetButtonFromString(b.c_str());
		return btn == SDL_CONTROLLER_BUTTON_INVALID ? UNKNOWN_CODE : PAD_CODE(0, btn);
	}
	SDL_Keycode k = SDL_GetKeyFromName(name.c_str());
	return k == SDLK_UNKNOWN ? UNKNOWN_CODE : (unsigned int)k;
}
