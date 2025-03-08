#include "menus/keys_menu.h"

#include <vector>

#include "core/config.h"
#include "core/text_data.h"
#include "engine/control_alias.h"
#include "engine/input.h"
#include "engine/key_translator.h"
#include "menus/txt_defines.h"

static const int ACTION_TEXT[NB_ACTIONS] = {TXT_UP, TXT_DOWN, TXT_LEFT, TXT_RIGHT, TXT_FIRE, TXT_JUMP, TXT_SPECIAL};

KeysMenu::KeysMenu(int scheme)
	: scheme_(scheme),
	  self_(scheme == SCHEME_KEYBOARD ? MenuType::Keys_Keyboard
			: scheme == SCHEME_MOUSE  ? MenuType::Keys_Mouse
									  : MenuType::Keys_Pad) {
	for (int a = 0; a < NB_ACTIONS; a++) items_.AddEntry(KeyText(a));
	items_.AddEntry(txt_data[TXT_RETURN]);
}

std::string KeysMenu::KeyText(int action) const { return txt_data[ACTION_TEXT[action]] + " = " + DIK_to_string(in.getAlias(scheme_ + action)); }

void KeysMenu::Refresh() {
	for (int a = 0; a < NB_ACTIONS; a++) items_.ChangeEntry(a, KeyText(a));
}

void KeysMenu::SetKey(unsigned int key) {
	int alias = scheme_ + items_.focused();
	// Two keyboard players share one keyboard, a key taken from the other binding moves there instead of driving both
	const std::vector<int> shared = scheme_ == SCHEME_PAD ? std::vector<int>{SCHEME_PAD} : std::vector<int>{SCHEME_KEYBOARD, SCHEME_MOUSE};
	for (int s : shared) {
		for (int a = 0; a < NB_ACTIONS; a++) {
			if (s + a != alias && in.getAlias(s + a) == key) in.setAlias(s + a, in.getAlias(alias));
		}
	}
	in.setAlias(alias, key);
	Refresh();
	state_ = State::Browsing;
}

int KeysMenu::ProcessEvent() {
	if (state_ == State::WaitingRelease) {
		if (in.heldCode() < 0) state_ = State::WaitingKey;
	} else if (state_ == State::WaitingKey) {
		// Menu Escape, so a pad can cancel a keyboard binding it cannot fill
		if (in.pressedKey(DIK_ESCAPE)) {
			Refresh();
			state_ = State::Browsing;
			return self_;
		}
		int key = in.heldCode(scheme_ == SCHEME_PAD ? DEV_PADS : scheme_ == SCHEME_MOUSE ? DEV_KEYS | DEV_MOUSE : DEV_KEYS);
		if (key >= 0 && scheme_ == SCHEME_PAD) {
			int b = key & 0x3FF;
			// Start and Back pause the game and leave menus
			if (b == SDL_CONTROLLER_BUTTON_START || b == SDL_CONTROLLER_BUTTON_BACK) return self_;
			key = PAD_CODE(0, b);
		}
		if (key >= 0) SetKey(key);
	} else {
		if (in.pressedKey(DIK_UP) || in.pressedAlias(SCHEME_KEYBOARD + ACTION_UP)) {
			items_.MoveUp();
		} else if (in.pressedKey(DIK_DOWN) || in.pressedAlias(SCHEME_KEYBOARD + ACTION_DOWN)) {
			items_.MoveDown();
		}
		if (in.pressedKey(DIK_RETURN) || in.pressedAlias(SCHEME_KEYBOARD + ACTION_FIRE)) {
			if (items_.focused() == (int)items_.size() - 1) return MenuType::Options;
			state_ = State::WaitingRelease;
			items_.ChangeEntry(items_.focused(), txt_data[ACTION_TEXT[items_.focused()]] + " = ...");
		}
	}
	return self_;
}
