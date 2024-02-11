#pragma once

#include "sprite.h"

class FondEcranneige : public Sprite {
   public:
	virtual void update() {
		ss_etape += 1;
		ss_etape %= 4;

		if (ss_etape == 0) {
			etape += 1;
			etape %= 4;
		}

		pic = pbk_niveau[45 + etape];
		updateADetruire();
	};
};
