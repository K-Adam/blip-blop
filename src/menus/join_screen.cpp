#include "menus/join_screen.h"

#include <string>

#include "level/world.h"
#include "core/assets.h"
#include "engine/half_tone.h"
#include "engine/key_translator.h"

bool JoinScreen::press(int scheme, int pad) {
	for (int i = 0; i < players_; i++) {
		if (joined_[i].scheme() == scheme && joined_[i].pad() == pad) return true;
	}
	if (players_ < 2) joined_[players_++] = SchemeController(scheme, pad);
	return false;
}

JoinScreen::Output JoinScreen::update() {
	if (in.pressedCode(DIK_ESCAPE)) return Output::Back;

	bool start = false;
	if (in.pressedCode(in.getAlias(SCHEME_KEYBOARD + ACTION_JUMP))) start |= press(SCHEME_KEYBOARD, 0);
	if (in.pressedCode(in.getAlias(SCHEME_MOUSE + ACTION_FIRE))) start |= press(SCHEME_MOUSE, 0);
	for (int s = 0; s < MAX_JOY; s++) {
		if (in.pressedCode(PAD_CODE(s, SDL_CONTROLLER_BUTTON_BACK))) return Output::Back;
		// A confirms like in every menu, jump sits on a trigger. Start reads as Escape elsewhere, here it begins
		if (in.pressedCode(PAD_CODE(s, SDL_CONTROLLER_BUTTON_A)) || in.pressedCode(PAD_CODE(s, SDL_CONTROLLER_BUTTON_START))) {
			start |= press(SCHEME_PAD, s);
		}
	}
	return start ? Output::Start : Output::Continue;
}

static std::string schemeName(const SchemeController& c) {
	if (c.scheme() == SCHEME_KEYBOARD) return "KEYBOARD";
	if (c.scheme() == SCHEME_MOUSE) return "MOUSE AND KEYBOARD";
	return "CONTROLLER " + std::to_string(c.pad() + 1);
}

void JoinScreen::draw() const {
	pbk_inter.pic("title")->PasteTo(backSurface, 0, 0);

	Rect r;
	r.top = 140;
	r.left = 40;
	r.right = 600;
	r.bottom = 360;
	halfTone(backSurface, &r);

	for (int i = 0; i < 2; i++) {
		std::string slot = "PLAYER " + std::to_string(i + 1) + " : " + (i < players_ ? schemeName(joined_[i]) : "...");
		fnt_menu.printC(backSurface, 320, 160 + 40 * i, slot);
	}

	unsigned int click = in.getAlias(SCHEME_MOUSE + ACTION_FIRE);
	std::string mouse = click == MOUSE_CODE(SDL_BUTTON_LEFT) ? "CLICK" : DIK_to_string(click);
	std::string join =
		"PRESS " + DIK_to_string(in.getAlias(SCHEME_KEYBOARD + ACTION_JUMP)) + ", " + mouse + " OR " + DIK_to_string(PAD_CODE(0, SDL_CONTROLLER_BUTTON_A));
	if (players_ < 2) fnt_menu.printC(backSurface, 320, 260, join + " TO JOIN");
	if (players_ > 0) fnt_menu.printC(backSurface, 320, 300, "PRESS AGAIN TO START");
}
