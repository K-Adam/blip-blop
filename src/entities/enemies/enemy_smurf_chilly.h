#pragma once

#include "entities/enemies/enemy_smurf.h"

class EnemySmurfChilly : public EnemySmurf {
   public:
	EnemySmurfChilly() { pv = 75; }

	inline virtual void affiche() {
		if (etat != ETAT_MEURE && etat != ETAT_CARBONISE) {
			switch (dir) {
				case SENS_GAUCHE:
					draw(x + 7, y - 18, pbk_ennemis[147]);
					EnemySmurf::affiche();
					draw(x, y - 20, pbk_ennemis[149]);
					break;
				case SENS_DROITE:
					draw(x - 7, y - 18, pbk_ennemis[148]);
					EnemySmurf::affiche();
					draw(x, y - 20, pbk_ennemis[150]);
					break;
			}
		} else
			EnemySmurf::affiche();
	}

	virtual void onAvance() {
		speed = 1 + rand() % 3;
		EnemySmurf::onAvance();
	}
};
