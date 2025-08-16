#pragma once

#include "entities/enemies/enemy_smurf.h"

class EnemySmurfVain : public EnemySmurf {
   public:
	EnemySmurfVain() {
		dy_saut = -6;
		speed = 1 + rand() % 2;
	}

	inline void affiche() override {
		EnemySmurf::affiche();
		if (etat != ETAT_MEURE && etat != ETAT_CARBONISE) {
			switch (dir) {
				case SENS_GAUCHE:
					draw(x + 3, y - 31, pbk_ennemis[151]);
					break;
				case SENS_DROITE:
					draw(x - 3, y - 31, pbk_ennemis[151]);
					break;
			}
		}
	}
};
