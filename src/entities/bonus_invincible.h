#pragma once

#include "entities/sprites.h"
#include "entities/bonus.h"

class BonusInvincible : public Bonus {
   public:
	BonusInvincible() {
		pic = pbk_misc[MISC::BONUS_INVINCIBLE];
		col_on = true;
	};

	void onPickup(Ball* c) override {
		c->invincible = 550;

		Bonus::onPickup(c);
	};
};
