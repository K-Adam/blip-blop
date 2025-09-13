#pragma once

#include "entities/enemies/enemy_smurf.h"

class EnemySmurfGlasses : public EnemySmurf {
   public:
	inline void render() override {
		EnemySmurf::render();
		if (status != ETAT_MEURE && status != ETAT_CARBONISE) {
			switch (dir) {
				case FACING_LEFT:
					draw(x - 4, y - 27, pbk_enemies[145]);
					break;
				case FACING_RIGHT:
					draw(x + 4, y - 27, pbk_enemies[146]);
					break;
			}
		}
	}

	void update() override {
		if (status == ETAT_SAUTE)
			speed = 2;
		else
			speed = 1;

		EnemySmurf::update();
	}
};
