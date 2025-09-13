#pragma once

#include "entities/ball.h"

class Blip : public Ball {
   public:
	Blip() : Ball(pbk_blip) { id_ball = ID_BLIP; }

	void rearme() override {
		weapon_id = ID_PM;
		ammo = 100;
		weapon_anim_delay = 3;
		nb_weapon_steps = 5;
		fire_interval = 5;
		gun_weight = 5;

		etape_arme = 0;
		ss_etape_arme = 0;
		etape_recul = 0;
		is_firing = false;
	}
};
