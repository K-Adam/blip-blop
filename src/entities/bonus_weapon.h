#pragma once

#include <algorithm>

#include "entities/bonus.h"

struct WeaponSpec {
	const char* pic;
	int sound, id, ammo, max_ammo;
};

class BonusWeapon : public Bonus {
   public:
	explicit BonusWeapon(const WeaponSpec& w) : w_(w) {
		pic = assets().pics.misc.pic(w.pic);
		col_on = true;
	}

	void onPickup(Ball* c) override {
		assets().sounds.misc.play(w_.sound);

		bool carried = c->carries(w_.id);
		c->ammo[w_.id] = std::min(c->ammo[w_.id] + w_.ammo, w_.max_ammo);
		if (!carried) c->equip(w_.id);

		Bonus::onPickup(c);
	}

   private:
	WeaponSpec w_;
};

class BonusPM : public BonusWeapon {
   public:
	BonusPM() : BonusWeapon({"icon_pm", 4, ID_PM, 200, 400}) {}
};

class BonusShotgun : public BonusWeapon {
   public:
	BonusShotgun() : BonusWeapon({"icon_shotgun", 0, ID_SHOTGUN, 20, 40}) {}
};

class BonusLaser : public BonusWeapon {
   public:
	BonusLaser() : BonusWeapon({"icon_laser", 3, ID_LASER, 750, 1500}) {}
};

class BonusLF : public BonusWeapon {
   public:
	BonusLF() : BonusWeapon({"icon_lf", 2, ID_LF, 1000, 2000}) {}
};
