#pragma once

#include "entities/sprite.h"

class ShotCruiser : public Sprite {
   public:
	int dx;

	ShotCruiser(int vx, int vy, int nb_pic);
	void update() override;
};
