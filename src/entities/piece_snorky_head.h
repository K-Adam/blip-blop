#pragma once

#include "entities/sprite.h"

class PieceSnorkyBaseHeadWhole : public Sprite {
   public:
	PieceSnorkyBaseHeadWhole();

	void update() override;
};

class PieceSnorkyBaseHeadBackPart : public Sprite {
   public:
	int dx;

	PieceSnorkyBaseHeadBackPart(int vx, int vy);

	void update() override;
};

class PieceSnorkyHyporiderBody : public Sprite {
   public:
	int dx;

	PieceSnorkyHyporiderBody(int vx);

	void update() override;
};

class PieceSnorkyBaseTrunk : public Sprite {
   public:
	int dx;

	PieceSnorkyBaseTrunk(int vx, int vy);

	void update() override;
};
