#pragma once

#include "entities/sprites.h"
#include "entities/bonus.h"

class BonusBeer : public Bonus {
   public:
	BonusBeer() {
		pic = pbk_misc[MISC::BONUS_BEER];
		col_on = true;
	};

	void onPickup(Ball* c) override {
		if (c->hp == 5) return;

		sbk_misc.play(5);

		c->hp += 2;

		if (c->hp > 5) c->hp = 5;

		Bonus::onPickup(c);
	};
};
