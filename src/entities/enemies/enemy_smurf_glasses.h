#pragma once

#include "entities/enemies/enemy_smurf.h"

class EnemySmurfGlasses : public EnemySmurf {
   public:
	inline void affiche() override {
		EnemySmurf::affiche();
		if (etat != ETAT_MEURE && etat != ETAT_CARBONISE) {
			switch (dir) {
				case SENS_GAUCHE:
					draw(x - 4, y - 27, pbk_ennemis[145]);
					break;
				case SENS_DROITE:
					draw(x + 4, y - 27, pbk_ennemis[146]);
					break;
			}
		}
	}

	void update() override {
		if (etat == ETAT_SAUTE)
			speed = 2;
		else
			speed = 1;

		EnemySmurf::update();
	}
};
