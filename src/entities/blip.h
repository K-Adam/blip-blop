#pragma once

#include "entities/ball.h"

class Blip : public Ball {
   public:
	Blip() : Ball(pbk_blip) { id_ball = ID_BLIP; }

	void rearm() override {
		weapon_id = ID_PM;
		ammo = 100;
		weapon_anim_delay = 3;
		nb_weapon_steps = 5;
		fire_interval = 5;
		gun_weight = 5;

		weapon_step = 0;
		weapon_tick = 0;
		recoil_step = 0;
		is_firing = false;
	}
};
