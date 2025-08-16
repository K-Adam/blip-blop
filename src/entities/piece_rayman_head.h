#pragma once

#include "entities/sprite.h"

class PieceRaymanHead : public Sprite {
   private:
	int dx;

   public:
	PieceRaymanHead(int vx, int vy);
	void update() override;
};
