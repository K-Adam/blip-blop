#pragma once

#include "entities/bonus.h"

class BonusBarrel : public Bonus {
   public:
	BonusBarrel() {
		pic = pbk_misc[47];
		col_on = true;
	};

	virtual void estPris(Ball* c) {
		sbk_misc.play(5);

		c->pv = 5;

		Bonus::estPris(c);
	};
};
