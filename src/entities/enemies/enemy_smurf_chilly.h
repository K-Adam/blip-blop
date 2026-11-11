#pragma once

#include "entities/enemies/enemy_smurf.h"

class EnemySmurfChilly : public EnemySmurf {
   public:
	EnemySmurfChilly() { hp = 75; }

	inline void render() override {
		if (status != STATE_DYING && status != STATE_CHARRED) {
			switch (dir) {
				case FACING_LEFT:
					draw(x + 7, y - 18, assets().pics.enemies.pic("smurf_chilly_scarf_left"));
					EnemySmurf::render();
					draw(x, y - 20, assets().pics.enemies.pic("smurf_chilly_hat_left"));
					break;
				case FACING_RIGHT:
					draw(x - 7, y - 18, assets().pics.enemies.pic("smurf_chilly_scarf_right"));
					EnemySmurf::render();
					draw(x, y - 20, assets().pics.enemies.pic("smurf_chilly_hat_right"));
					break;
			}
		} else
			EnemySmurf::render();
	}

	void onWalk() override {
		speed = 1 + rnd() % 3;
		EnemySmurf::onWalk();
	}
};
