#pragma once

#include "entities/sprite.h"

class ShotCruiser : public Sprite {
   public:
	int dx;

	ShotCruiser(int vx, int vy, const Picture* p);
	void update() override;
};
