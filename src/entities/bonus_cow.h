#pragma once

#include "entities/sprites.h"
#include "entities/bonus.h"

class BonusCow : public Bonus {
   public:
	BonusCow() {
		pic = pbk_misc[MISC::BONUS_COW];
		col_on = true;
	};

	void onPickup(Ball* c) override {
		sbk_misc.play(1);

		c->nb_cow_bomb += 1;

		Bonus::onPickup(c);
	};
};
