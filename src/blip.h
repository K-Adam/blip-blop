#pragma once

#include "couille.h"

class Blip : public Couille {
   public:
	Blip() : Couille(pbk_blip) { id_couille = ID_BLIP; }

	virtual void rearme() {
		id_arme = ID_PM;
		ammo = 100;
		latence_arme = 3;
		nb_etape_arme = 5;
		cadence_arme = 5;
		poid_arme = 5;

		etape_arme = 0;
		ss_etape_arme = 0;
		etape_recul = 0;
		tire = false;
	}
};
