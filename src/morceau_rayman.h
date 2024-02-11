#pragma once

#include "sprite.h"

class MorceauRayman : public Sprite {
   public:
	int dx;
	int n_pic;

	MorceauRayman(int m_pic, int vx, int vy);

	virtual void update();
};
