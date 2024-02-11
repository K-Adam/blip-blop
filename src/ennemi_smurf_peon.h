#pragma once

#include "ennemi_smurf.h"

class EnnemiSmurfPeon : public EnnemiSmurf {
   public:
	EnnemiSmurfPeon() { pv = 125; }

	inline virtual void affiche() {
		EnnemiSmurf::affiche();
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
