#pragma once

#include "entities/shots/shot.h"

class ShotSnorkyBase2 : public Shot {
   public:
	int dx;

	ShotSnorkyBase2(int vx, int vy);

	virtual int degats() const { return 1; };

	virtual int enflame() const { return 0; };

	virtual void setDir(int d) { dir = d; };

	virtual void update();
};
