#pragma once

#include "entities/sprite.h"

class PieceSonicRing : public Sprite {
   public:
	int dx;

	PieceSonicRing(int vx, int vy);

	void update() override;
};
