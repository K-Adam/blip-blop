#include "menus/character_selection.h"

#include "core/app.h"
#include "level/world.h"
#include "engine/control_alias.h"
#include "engine/input.h"

CharacterSelection::CharacterSelection()
	: phase_tick_(0),
	  phase_(0),
	  x_back1_(0),
	  x_back2_(640),
	  y_select_(-50),
	  x_name_(1040),
	  y_name_(255),
	  step_(APPEAR_BLIP),
	  pic_select_((settings().lang == LANG_UK) ? "select_title_en" : "select_title_fr"),
	  x_charac_(-300),
	  y_charac_(255) {}

CharacterSelection::Output CharacterSelection::update() {
	phase_tick_ += 1;
	phase_tick_ %= 10;

	if (phase_tick_ == 0) {
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

	if (step_ == APPEAR_BLIP) {
		if (x_charac_ < 240) {
			x_charac_ += 20;
		} else if (in.pressedKey(SDLK_RIGHT) || in.pressedAlias(SCHEME_KEYBOARD + ACTION_RIGHT) || in.pressedAlias(SCHEME_MOUSE + ACTION_RIGHT)) {
			step_ = DONE_BLIP;
		}

		if (x_name_ > 500) {
			x_name_ -= 20;
		}

		if (x_charac_ >= 240 &&
			(in.pressedKey(SDLK_RETURN) || in.pressedAlias(SCHEME_KEYBOARD + ACTION_FIRE) || in.pressedAlias(SCHEME_KEYBOARD + ACTION_JUMP))) {
			return Output::Blip;
		}
	} else if (step_ == DONE_BLIP) {
		if (x_charac_ > -300) {
			x_charac_ -= 20;
		}

		if (x_name_ < 1040) {
			x_name_ += 20;
		}

		if (x_charac_ <= -300) {
			step_ = APPEAR_BLOP;
			x_charac_ = 930;
			x_name_ = -500;
			y_charac_ = 320;
		}
	} else if (step_ == APPEAR_BLOP) {
		if (x_charac_ > 310) {
			x_charac_ -= 20;
		} else if (in.pressedKey(SDLK_LEFT) || in.pressedAlias(SCHEME_KEYBOARD + ACTION_LEFT) || in.pressedAlias(SCHEME_MOUSE + ACTION_LEFT)) {
			step_ = DONE_BLOP;
		}

		if (x_name_ < 120) {
			x_name_ += 20;
		}

		if (x_charac_ <= 310 &&
			(in.pressedKey(SDLK_RETURN) || in.pressedAlias(SCHEME_KEYBOARD + ACTION_FIRE) || in.pressedAlias(SCHEME_KEYBOARD + ACTION_JUMP))) {
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
			step_ = APPEAR_BLIP;
			x_charac_ = -300;
			x_name_ = 1040;
			y_charac_ = 255;
		}
	}

	return Output::Continue;
}

void CharacterSelection::draw() {
	assets().pics.inter.anim("select_stars")[0]->PasteTo(graphics.back(), x_back1_, 0);
	assets().pics.inter.anim("select_stars")[1]->PasteTo(graphics.back(), x_back2_, 0);

	assets().pics.inter.pic(pic_select_)->BlitTo(graphics.back(), 320, y_select_);

	if (step_ == APPEAR_BLIP) {
		if (phase_) {
			assets().pics.inter.pic("select_arrow_right")->BlitTo(graphics.back(), 620, 257);
		}

		assets().pics.inter.pic("select_blip")->BlitTo(graphics.back(), x_charac_, y_charac_);
		assets().pics.inter.pic("name_blip")->BlitTo(graphics.back(), x_name_, y_name_);
	} else if (step_ == DONE_BLIP) {
		assets().pics.inter.pic("select_blip")->BlitTo(graphics.back(), x_charac_, y_charac_);
		assets().pics.inter.pic("name_blip")->BlitTo(graphics.back(), x_name_, y_name_);
	} else if (step_ == APPEAR_BLOP) {
		if (phase_) {
			assets().pics.inter.pic("select_arrow_left")->BlitTo(graphics.back(), 20, 254);
		}

		assets().pics.inter.pic("select_blop")->BlitTo(graphics.back(), x_charac_, y_charac_);
		assets().pics.inter.pic("name_blop")->BlitTo(graphics.back(), x_name_, y_name_);
	} else {
		assets().pics.inter.pic("select_blop")->BlitTo(graphics.back(), x_charac_, y_charac_);
		assets().pics.inter.pic("name_blop")->BlitTo(graphics.back(), x_name_, y_name_);
	}
}
