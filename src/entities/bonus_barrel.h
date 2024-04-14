#pragma once

#include "entities/bonus.h"

class BonusTonneau : public Bonus {
   public:
	BonusTonneau() {
		pic = pbk_misc[47];
		col_on = true;
	};

	virtual void estPris(Couille* c) {
		sbk_misc.play(5);

		c->pv = 5;

		Bonus::estPris(c);
	};
};
