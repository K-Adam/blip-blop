#pragma once

#include "entities/ball.h"

class Blop : public Ball {
   public:
	Blop() : Ball(assets().pics.blop) { id_ball = ID_BLOP; };

	void rearm() override {
		ammo[ID_SHOTGUN] = 10;
		equip(ID_SHOTGUN);
	}
};
