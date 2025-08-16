#pragma once

#include "entities/shots/shot.h"

#define DELAY_ACCELERATION 20

class ShotHadoken : public Shot {
   public:
	int degats() const override { return 1; };

	int enflame() const override { return 0; };

	void setDir(int d) override { dir = d; };

	void update() override {
		static const int SPEED = 8;

		ss_etape += 1;
		ss_etape %= 2;

		if (ss_etape == 0) {
			etape += 1;
			etape %= 3;
		}

		if (etape == 2) {
			pic = NULL;
		} else {
			if (dir == SENS_DROITE) {
				x += SPEED;
				pic = pbk_ennemis[219 + etape];
			} else {
				x -= SPEED;
				pic = pbk_ennemis[221 + etape];
			}
		}

		if (mur_opaque(x, y)) a_detruire = true;

		updateADetruire();
		colFromPic();
	};
};
