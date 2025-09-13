#pragma once

#include "entities/ball.h"

class Blop : public Ball {
   public:
	Blop() : Ball(pbk_blop) { id_ball = ID_BLOP; };

	void rearme() override {
		weapon_id = ID_SHOTGUN;
		ammo = 10;
		weapon_anim_delay = 3;
		nb_weapon_steps = 15;
		fire_interval = 45;
		gun_weight = 1;

		etape_arme = 0;
		ss_etape_arme = 0;
		etape_recul = 0;
		is_firing = false;
	}
};
