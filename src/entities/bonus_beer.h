#pragma once

#include "entities/bonus.h"

class BonusBeer : public Bonus {
   public:
	BonusBeer() {
		pic = pbk_misc.pic("bonus_beer");
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
