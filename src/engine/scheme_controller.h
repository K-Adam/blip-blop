#pragma once

#include <cstdlib>

#include "engine/control_alias.h"
#include "engine/controller.h"
#include "engine/input.h"

// A player driven by one control scheme. Pad schemes read the pad the player joined with
class SchemeController : public Controller {
   public:
	SchemeController(int scheme = SCHEME_KEYBOARD, int pad = 0) : scheme_(scheme), pad_(pad) {}

	int scheme() const { return scheme_; }
	int pad() const { return pad_; }

	unsigned int code(int action) const {
		unsigned int c = in.getAlias(scheme_ + action);
		return scheme_ == SCHEME_PAD && c ? PAD_CODE(pad_, c & 0x3FF) : c;
	}

	int leftHeld() const override { return in.scanCode(code(ACTION_LEFT)); }
	int upHeld() const override { return in.scanCode(code(ACTION_UP)); }
	int rightHeld() const override { return in.scanCode(code(ACTION_RIGHT)); }
	int downHeld() const override { return in.scanCode(code(ACTION_DOWN)); }
	int fire() const override { return in.scanCode(code(ACTION_FIRE)); }
	int jump() const override { return in.scanCode(code(ACTION_JUMP)); }
	int super() const override { return in.scanCode(code(ACTION_SUPER)); }
	int nextWeapon() const override { return in.scanCode(code(ACTION_NEXT_WEAPON)); }
	int prevWeapon() const override { return in.scanCode(code(ACTION_PREV_WEAPON)); }

	void rumble(int strength, int ms) const override {
		if (scheme_ == SCHEME_PAD) in.rumble(pad_, strength, ms);
	}

	void aim(int px, int py, int& vx, int& vy) const override {
		if (scheme_ == SCHEME_PAD) {
			// Twin stick convention, the move directions take over while the right stick rests
			in.padStick(pad_, vx, vy);
			if (std::abs(vx) > DEAD_ZONE || std::abs(vy) > DEAD_ZONE) return;
		}
		if (scheme_ != SCHEME_MOUSE) return Controller::aim(px, py, vx, vy);
		int dx = in.mouseX() - px;
		int dy = in.mouseY() - py;
		// Near the player the angle jitters, keep the last direction
		if (dx * dx + dy * dy > 16 * 16) {
			last_vx_ = dx;
			last_vy_ = dy;
		}
		vx = last_vx_;
		vy = last_vy_;
	}

   private:
	int scheme_;
	int pad_;
	mutable int last_vx_ = 1;
	mutable int last_vy_ = 0;
};
