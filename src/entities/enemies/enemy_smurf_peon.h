#pragma once

#include "entities/enemies/enemy_smurf.h"

class EnemySmurfPeon : public EnemySmurf {
   public:
	EnemySmurfPeon() { pv = 125; }

	inline void affiche() override {
		EnemySmurf::affiche();
		if ((etat != ETAT_MEURE && etat != ETAT_CARBONISE) || (etat == ETAT_MEURE && etape <= 2)) {
			switch (dir) {
				case SENS_GAUCHE:
					draw(x, y - 32, pbk_ennemis[153]);
					break;
				case SENS_DROITE:
					draw(x, y - 32, pbk_ennemis[154]);
					break;
			}
		}
	}
};
