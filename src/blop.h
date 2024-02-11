#pragma once

#include "couille.h"

class Blop : public Couille {
   public:
	Blop() : Couille(pbk_blop) { id_couille = ID_BLOP; };

	virtual void rearme() {
		id_arme = ID_FUSIL;
		ammo = 10;
		latence_arme = 3;
		nb_etape_arme = 15;
		cadence_arme = 45;
		poid_arme = 1;

		etape_arme = 0;
		ss_etape_arme = 0;
		etape_recul = 0;
		tire = false;
	}
};
