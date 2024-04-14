#include <string>

#include <cstring>
#include "core/config.h"
#include "engine/input.h"
#include "engine/key_translator.h"

static std::string DIK_to_string_us(int n);
static std::string DIK_to_string_fr(int n);

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
		default: {
			const char* s = d < SDL_CONTROLLER_BUTTON_MAX ? SDL_GameControllerGetStringForButton(SDL_GameControllerButton(d)) : nullptr;
			name = s ? s : std::to_string(d);
			for (char& c : name) c = char(toupper(c));
		}
	}
	return std::string(fr ? "MANETTE" : "PAD") + std::to_string(n >> 10 & 0xFF) + " " + name;
}

std::string DIK_to_string(int n) {
	if (lang_type == LANG_FR) {
		return DIK_to_string_fr(n);
	} else {
		return DIK_to_string_us(n);
	}
}

std::string DIK_to_string_us(int n) {
	if (n >> 10 & 0xFF) return pad_to_string(n, false);

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
	if (n >> 10 & 0xFF) return pad_to_string(n, true);

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
