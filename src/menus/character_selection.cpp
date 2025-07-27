#include "menus/character_selection.h"

#include "core/config.h"
#include "core/globals.h"
#include "engine/control_alias.h"
#include "engine/input.h"

CharacterSelection::CharacterSelection()
	: et_phase_(0),
	  phase_(0),
	  x_back1_(0),
	  x_back2_(640),
	  y_select_(-50),
	  x_name_(1040),
	  y_name_(255),
	  step_(APPAR_BLIP),
	  pic_select_((lang_type == LANG_UK) ? 10 : 11),
	  x_charac_(-300),
	  y_charac_(255) {}

CharacterSelection::Output CharacterSelection::update() {
	et_phase_ += 1;
	et_phase_ %= 10;

	if (et_phase_ == 0) {
		phase_ = !phase_;
	}

	x_back1_ -= 20;
	x_back2_ -= 20;

	if (x_back1_ == -640) {
		x_back1_ = 640;
	}
	if (x_back2_ == -640) {
		x_back2_ = 640;
	}

	if (y_select_ < 50) {
		y_select_ += 10;
	}

	if (step_ == APPAR_BLIP) {
		if (x_charac_ < 240) {
			x_charac_ += 20;
		} else if (in.pressedKey(DIK_RIGHT) || in.pressedAlias(SCHEME_KEYBOARD + ACTION_RIGHT) || in.pressedAlias(SCHEME_MOUSE + ACTION_RIGHT)) {
			step_ = FINI_BLIP;
		}

		if (x_name_ > 500) {
			x_name_ -= 20;
		}

		if (x_charac_ >= 240 &&
			(in.pressedKey(DIK_RETURN) || in.pressedAlias(SCHEME_KEYBOARD + ACTION_FIRE) || in.pressedAlias(SCHEME_KEYBOARD + ACTION_JUMP))) {
			return Output::Blip;
		}
	} else if (step_ == FINI_BLIP) {
		if (x_charac_ > -300) {
			x_charac_ -= 20;
		}

		if (x_name_ < 1040) {
			x_name_ += 20;
		}

		if (x_charac_ <= -300) {
			step_ = APPAR_BLOP;
			x_charac_ = 930;
			x_name_ = -500;
			y_charac_ = 320;
		}
	} else if (step_ == APPAR_BLOP) {
		if (x_charac_ > 310) {
			x_charac_ -= 20;
		} else if (in.pressedKey(DIK_LEFT) || in.pressedAlias(SCHEME_KEYBOARD + ACTION_LEFT) || in.pressedAlias(SCHEME_MOUSE + ACTION_LEFT)) {
			step_ = FINI_BLOP;
		}

		if (x_name_ < 120) {
			x_name_ += 20;
		}

		if (x_charac_ <= 310 &&
			(in.pressedKey(DIK_RETURN) || in.pressedAlias(SCHEME_KEYBOARD + ACTION_FIRE) || in.pressedAlias(SCHEME_KEYBOARD + ACTION_JUMP))) {
			return Output::Blop;
		}
	} else {
		if (x_charac_ < 930) {
			x_charac_ += 20;
		}

		if (x_name_ > -500) {
			x_name_ -= 20;
		}

		if (x_charac_ >= 930) {
			step_ = APPAR_BLIP;
			x_charac_ = -300;
			x_name_ = 1040;
			y_charac_ = 255;
		}
	}

	return Output::Continue;
}

void CharacterSelection::draw() {
	pbk_inter[6]->PasteTo(backSurface, x_back1_, 0);
	pbk_inter[7]->PasteTo(backSurface, x_back2_, 0);

	pbk_inter[pic_select_]->BlitTo(backSurface, 320, y_select_);

	if (step_ == APPAR_BLIP) {
		if (phase_) {
			pbk_inter[4]->BlitTo(backSurface, 620, 257);
		}

		pbk_inter[2]->BlitTo(backSurface, x_charac_, y_charac_);
		pbk_inter[8]->BlitTo(backSurface, x_name_, y_name_);
	} else if (step_ == FINI_BLIP) {
		pbk_inter[2]->BlitTo(backSurface, x_charac_, y_charac_);
		pbk_inter[8]->BlitTo(backSurface, x_name_, y_name_);
	} else if (step_ == APPAR_BLOP) {
		if (phase_) {
			pbk_inter[5]->BlitTo(backSurface, 20, 254);
		}

		pbk_inter[3]->BlitTo(backSurface, x_charac_, y_charac_);
		pbk_inter[9]->BlitTo(backSurface, x_name_, y_name_);
	} else {
		pbk_inter[3]->BlitTo(backSurface, x_charac_, y_charac_);
		pbk_inter[9]->BlitTo(backSurface, x_name_, y_name_);
	}
}
