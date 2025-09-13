#pragma once

#include <algorithm>

#include "entities/bonus.h"

struct WeaponSpec {
	int pic, sound, id, ammo, max_ammo, anim_steps, shot_interval, turn_delay;
	bool light;
};

class BonusWeapon : public Bonus {
   public:
	explicit BonusWeapon(const WeaponSpec& w) : w_(w) {
		pic = pbk_misc[w.pic];
		col_on = true;
	}

	void onPickup(Ball* c) override {
		// A light weapon must not replace a well stocked flamethrower or laser
		if (w_.light && (c->weapon_id == ID_LF || c->weapon_id == ID_LASER) && c->ammo >= 100) return;

		sbk_misc.play(w_.sound);

		if (c->weapon_id == w_.id) {
			c->ammo = std::min(c->ammo + w_.ammo, w_.max_ammo);
		} else {
			c->weapon_id = w_.id;
			c->ammo = w_.ammo;
			c->weapon_anim_delay = 3;
			c->nb_weapon_steps = w_.anim_steps;
			c->fire_interval = w_.shot_interval;
			c->gun_turn_delay = w_.turn_delay;

			c->weapon_step = 0;
			c->weapon_tick = 0;
			c->recoil_step = 0;
			c->is_firing = false;
		}

		Bonus::onPickup(c);
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
	BonusShotgun() : BonusWeapon({9, 0, ID_SHOTGUN, 20, 40, 15, 45, 1, true}) {}
};

class BonusLaser : public BonusWeapon {
   public:
	BonusLaser() : BonusWeapon({10, 3, ID_LASER, 750, 1500, 4, 1, 1, false}) {}
};

class BonusLF : public BonusWeapon {
   public:
	BonusLF() : BonusWeapon({11, 2, ID_LF, 1000, 2000, 5, 1, 5, false}) {}
};
