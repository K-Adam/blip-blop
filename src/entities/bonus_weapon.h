#pragma once

#include <algorithm>

#include "entities/bonus.h"

struct WeaponSpec {
	int pic, sound, id, ammo, max_ammo, nb_etape, cadence, poid;
	bool light;
};

class BonusWeapon : public Bonus {
   public:
	explicit BonusWeapon(const WeaponSpec& w) : w_(w) {
		pic = pbk_misc[w.pic];
		col_on = true;
	}

	void estPris(Ball* c) override {
		// A light weapon must not replace a well stocked flamethrower or laser
		if (w_.light && (c->id_arme == ID_LF || c->id_arme == ID_LASER) && c->ammo >= 100) return;

		sbk_misc.play(w_.sound);

		if (c->id_arme == w_.id) {
			c->ammo = std::min(c->ammo + w_.ammo, w_.max_ammo);
		} else {
			c->id_arme = w_.id;
			c->ammo = w_.ammo;
			c->latence_arme = 3;
			c->nb_etape_arme = w_.nb_etape;
			c->cadence_arme = w_.cadence;
			c->poid_arme = w_.poid;

			c->etape_arme = 0;
			c->ss_etape_arme = 0;
			c->etape_recul = 0;
			c->tire = false;
		}

		Bonus::estPris(c);
	}

   private:
	WeaponSpec w_;
};

class BonusPM : public BonusWeapon {
   public:
	BonusPM() : BonusWeapon({8, 4, ID_PM, 200, 400, 5, 5, 5, true}) {}
};

class BonusShotgun : public BonusWeapon {
   public:
	BonusShotgun() : BonusWeapon({9, 0, ID_FUSIL, 20, 40, 15, 45, 1, true}) {}
};

class BonusLaser : public BonusWeapon {
   public:
	BonusLaser() : BonusWeapon({10, 3, ID_LASER, 750, 1500, 4, 1, 1, false}) {}
};

class BonusLF : public BonusWeapon {
   public:
	BonusLF() : BonusWeapon({11, 2, ID_LF, 1000, 2000, 5, 1, 5, false}) {}
};
