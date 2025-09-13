#pragma once

#include "entities/bonus.h"

class BonusCow : public Bonus {
   public:
	BonusCow() {
		pic = pbk_misc[48];
		col_on = true;
	};

	void onPickup(Ball* c) override {
		sbk_misc.play(1);

		c->nb_cow_bomb += 1;

		Bonus::onPickup(c);
	};
};
