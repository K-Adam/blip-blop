#pragma once

#include "entities/sprite.h"

// FIXME: shouldn't it inherit Giclure?
class GoreGiclure : public Sprite {
   public:
	int dx;

	GoreGiclure(int vx, int vy);

	virtual void update();
};
