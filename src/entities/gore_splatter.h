#pragma once

#include "entities/sprite.h"

// FIXME: shouldn't it inherit Splatter?
class GoreSplatter : public Sprite {
   public:
	int dx;

	GoreSplatter(int vx, int vy);

	virtual void update();
};
