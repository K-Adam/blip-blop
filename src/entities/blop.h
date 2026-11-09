#pragma once

#include "entities/ball.h"

class Blop : public Ball {
   public:
	Blop() : Ball(pbk_blop) { id_ball = ID_BLOP; };

	void rearm() override {
		weapon_id = ID_SHOTGUN;
		ammo = 10;
		weapon_anim_delay = 3;
		nb_weapon_steps = 15;
		fire_interval = 45;
		gun_turn_delay = 1;

		weapon_step = 0;
		weapon_tick = 0;
		recoil_step = 0;
		is_firing = false;
	}
};
