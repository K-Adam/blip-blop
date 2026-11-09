#pragma once

#include "entities/sprite.h"

class PieceRayman : public Sprite {
   public:
	int dx;
	int piece;	// Which part landed, told to the level through game_flag[2]

	PieceRayman(const Picture* p, int piece, int vx, int vy);

	void update() override;
};
