#pragma once

#include "entities/bonus.h"

class BonusInvincible : public Bonus {
   public:
	BonusInvincible() {
		pic = pbk_misc[12];
		col_on = true;
	};

	virtual void estPris(Couille* c) {
		c->invincible = 550;

		Bonus::estPris(c);
	};
};
