#pragma once

#include "entities/sprite.h"

class PieceSnorkyBaseHeadWhole : public Sprite {
   public:
	PieceSnorkyBaseHeadWhole();

	virtual void update();
};

class PieceSnorkyBaseHeadBackPart : public Sprite {
   public:
	int dx;

	PieceSnorkyBaseHeadBackPart(int vx, int vy);

	virtual void update();
};

class PieceSnorkyHyporiderBody : public Sprite {
   public:
	int dx;

	PieceSnorkyHyporiderBody(int vx);

	virtual void update();
};

class PieceSnorkyBaseTrunk : public Sprite {
   public:
	int dx;

	PieceSnorkyBaseTrunk(int vx, int vy);

	virtual void update();
};
