#pragma once

#include "entities/enemies/enemy_smurf.h"

class EnemySmurfVain : public EnemySmurf {
   public:
	EnemySmurfVain() {
		dy_saut = -6;
		speed = 1 + rand() % 2;
	}

	inline void render() override {
		EnemySmurf::render();
		if (status != ETAT_MEURE && status != ETAT_CARBONISE) {
			switch (dir) {
				case FACING_LEFT:
					draw(x + 3, y - 31, pbk_enemies[151]);
					break;
				case FACING_RIGHT:
					draw(x - 3, y - 31, pbk_enemies[151]);
					break;
			}
		}
	}
};
