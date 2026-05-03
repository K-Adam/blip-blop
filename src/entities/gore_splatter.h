#pragma once

#include "entities/sprite.h"

class GoreSplatter : public Sprite {
   public:
	int dx;

	GoreSplatter(int vx, int vy);

	void update() override;
};
