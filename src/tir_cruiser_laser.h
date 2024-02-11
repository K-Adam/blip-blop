#pragma once

#include "sprite.h"

class TirCruiserLaser : public Sprite {
   public:
	int dx;
	int npic;

	TirCruiserLaser(int vx, int nb_pic);
	virtual void update();
};
