#pragma once

#include "entities/shots/shot.h"

#define DELAY_ACCELERATION 20

class ShotHadoken : public Shot {
   public:
	int damage() const override { return 1; };

	int enflame() const override { return 0; };

	void setDir(int d) override { dir = d; };

	void update() override {
		static const int SPEED = 8;

		step(2, 3);

		if (anim_step == 2) {
			pic = NULL;
		} else {
			if (dir == FACING_RIGHT) {
				x += SPEED;
				pic = assets().pics.enemies.anim("smurf_hefty_hadoken_right")[anim_step];
			} else {
				x -= SPEED;
				pic = assets().pics.enemies.anim("smurf_hefty_hadoken_left")[anim_step];
			}
		}

		if (level().terrain.solid_wall(x, y)) destroyed = true;

		updateDestroyed();
		colFromPic();
	};
};
