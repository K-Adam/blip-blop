#pragma once

#include "entities/sprite.h"

class PieceRaymanHead : public Sprite {
   private:
	int dx;

   public:
	// methodes "habituelles"
	PieceRaymanHead(int vx, int vy);
	virtual void update();
};
