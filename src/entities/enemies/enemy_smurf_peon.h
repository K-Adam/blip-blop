#pragma once

#include "entities/enemies/enemy_smurf.h"

class EnemySmurfPeon : public EnemySmurf {
   public:
	EnemySmurfPeon() { hp = 125; }

	inline void render() override {
		EnemySmurf::render();
		if ((status != ETAT_MEURE && status != ETAT_CARBONISE) || (status == ETAT_MEURE && anim_step <= 2)) {
			switch (dir) {
				case FACING_LEFT:
					draw(x, y - 32, pbk_enemies[153]);
					break;
				case FACING_RIGHT:
					draw(x, y - 32, pbk_enemies[154]);
					break;
			}
		}
	}
};
