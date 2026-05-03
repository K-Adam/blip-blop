#pragma once

#include "entities/sprites.h"
#include "entities/bonus.h"

class BonusBarrel : public Bonus {
   public:
	BonusBarrel() {
		pic = pbk_misc[MISC::BONUS_BARREL];
		col_on = true;
	};

	void onPickup(Ball* c) override {
		sbk_misc.play(5);

		c->hp = 5;

		Bonus::onPickup(c);
	};
};
