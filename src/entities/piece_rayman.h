#pragma once

#include "entities/sprite.h"

class PieceRayman : public Sprite {
   public:
	int dx;
	int n_pic;

	PieceRayman(int m_pic, int vx, int vy);

	virtual void update();
};
