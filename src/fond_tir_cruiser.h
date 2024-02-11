#pragma once

#include "sprite.h"

class TirCruiser : public Sprite {
   public:
	int dx;

	TirCruiser(int vx, int vy, int nb_pic);
	virtual void update();
};
