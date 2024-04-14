#pragma once

#include "entities/sprite.h"

class MorceauSonicRing : public Sprite {
   public:
	int dx;

	MorceauSonicRing(int vx, int vy);

	virtual void update();
};
