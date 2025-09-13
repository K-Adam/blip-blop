#pragma once

#include "entities/bonus.h"

class BonusBarrel : public Bonus {
   public:
	BonusBarrel() {
		pic = pbk_misc[47];
		col_on = true;
	};

	void onPickup(Ball* c) override {
		sbk_misc.play(5);

		c->hp = 5;

		Bonus::onPickup(c);
	};
};
