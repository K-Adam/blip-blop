#pragma once

#include "entities/ball.h"

class Blip : public Ball {
   public:
	Blip() : Ball(assets().pics.blip) { id_ball = ID_BLIP; }

	void rearm() override {
		ammo[ID_PM] = 100;
		equip(ID_PM);
	}
};
