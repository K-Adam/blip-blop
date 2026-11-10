#include "menus/keys_menu.h"

#include <vector>

#include "core/app.h"
#include "engine/control_alias.h"
#include "engine/input.h"
#include "engine/key_translator.h"
#include "menus/txt_defines.h"

static const int ACTION_TEXT[NB_ACTIONS] = {TXT_UP, TXT_DOWN, TXT_LEFT, TXT_RIGHT, TXT_FIRE, TXT_JUMP, TXT_SPECIAL, TXT_NEXT_WEAPON, TXT_PREV_WEAPON};

KeysMenu::KeysMenu(int scheme)
	: scheme_(scheme),
	  self_(scheme == SCHEME_KEYBOARD ? MenuType::Keys_Keyboard
			: scheme == SCHEME_MOUSE  ? MenuType::Keys_Mouse
									  : MenuType::Keys_Pad),
	  // UP is action 0 and the mouse scheme aims instead
	  first_(scheme == SCHEME_MOUSE ? 1 : 0) {
	for (int a = first_; a < NB_ACTIONS; a++) items_.AddEntry(KeyText(a));
	items_.AddEntry(texts()[TXT_RETURN]);
}

std::string KeysMenu::KeyText(int action) const {
	return texts()[ACTION_TEXT[action]] + " = " + DIK_to_string(in.getAlias(scheme_ + action), settings().lang == LANG_FR);
}

void KeysMenu::Refresh() {
	for (int a = first_; a < NB_ACTIONS; a++) items_.ChangeEntry(a - first_, KeyText(a));
}

void KeysMenu::SetKey(unsigned int key) {
	int alias = scheme_ + first_ + items_.focused();
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
		if (items_.Navigate()) {
			if (items_.focused() == (int)items_.size() - 1) return MenuType::Options;
			state_ = State::WaitingRelease;
			items_.ChangeEntry(items_.focused(), texts()[ACTION_TEXT[first_ + items_.focused()]] + " = ...");
		}
	}
	return self_;
}
