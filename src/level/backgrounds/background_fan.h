#pragma once

#include "entities/sprite.h"

class BackgroundFan : public Sprite {
   public:
	void update() override {
		ss_etape += 1;
		ss_etape %= 3;

		if (ss_etape == 0) {
			etape += 1;
			etape %= 4;
		}

		pic = pbk_niveau[3 + etape];

		if (x < offset - 100) a_detruire = true;
	};
};
