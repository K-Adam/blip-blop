#pragma once

#include "entities/bonus.h"

class BonusInvincible : public Bonus {
   public:
	BonusInvincible() {
		pic = pbk_misc.pic("bonus_invincible");
		col_on = true;
	};

	void onPickup(Ball* c) override {
		c->invincible = 550;

		Bonus::onPickup(c);
	};
};
