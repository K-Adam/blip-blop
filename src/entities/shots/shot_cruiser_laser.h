#pragma once

#include "entities/sprite.h"

class ShotCruiserLaser : public Sprite {
   public:
	int dx;
	int npic;

	ShotCruiserLaser(int vx, int nb_pic);
	void update() override;
};
