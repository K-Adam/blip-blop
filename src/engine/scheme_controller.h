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

	int gauche() const override { return in.scanCode(code(ACTION_LEFT)); }
	int haut() const override { return in.scanCode(code(ACTION_UP)); }
	int droite() const override { return in.scanCode(code(ACTION_RIGHT)); }
	int bas() const override { return in.scanCode(code(ACTION_DOWN)); }
	int fire() const override { return in.scanCode(code(ACTION_FIRE)); }
	int saut() const override { return in.scanCode(code(ACTION_JUMP)); }
	int super() const override { return in.scanCode(code(ACTION_SUPER)); }

	void aim(int px, int py, int& ax, int& ay) const override {
		if (scheme_ != SCHEME_MOUSE) return Controller::aim(px, py, ax, ay);
		int dx = in.mouseX() - px;
		int dy = in.mouseY() - py;
		// Near the player the angle jitters, keep the last direction
		if (dx * dx + dy * dy > 16 * 16) {
			// tan(22.5 degrees) splits the circle into 8 sectors
			last_ax_ = std::abs(dx) * 1000 > std::abs(dy) * 414 ? (dx > 0 ? 1 : -1) : 0;
			last_ay_ = std::abs(dy) * 1000 > std::abs(dx) * 414 ? (dy > 0 ? 1 : -1) : 0;
		}
		ax = last_ax_;
		ay = last_ay_;
	}

   private:
	int scheme_;
	int pad_;
	mutable int last_ax_ = 1;
	mutable int last_ay_ = 0;
};
