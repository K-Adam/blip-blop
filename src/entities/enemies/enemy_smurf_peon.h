#pragma once

#include "entities/enemies/enemy_smurf.h"

class EnemySmurfPeon : public EnemySmurf {
   public:
	EnemySmurfPeon() { hp = 125; }

	inline void render() override {
		EnemySmurf::render();
		if ((status != STATE_DYING && status != STATE_CHARRED) || (status == STATE_DYING && anim_step <= 2)) {
			switch (dir) {
				case FACING_LEFT:
					draw(x, y - 32, pbk_enemies.pic("smurf_peon_hat_left"));
					break;
				case FACING_RIGHT:
					draw(x, y - 32, pbk_enemies.pic("smurf_peon_hat_right"));
					break;
			}
		}
	}
};
