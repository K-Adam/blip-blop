#pragma once

#include "entities/shots/shot.h"

#define GRAVITE_FIREBALL 15
#define LATENCE_FIREBALL 10

class ShotMarioFireballVertical : public Shot {
   public:
	int dx;
	int x_cible;

	ShotMarioFireballVertical(int vx, int xc);

	virtual int degats() const { return 1; };

	virtual int enflame() const { return 0; };

	virtual void setDir(int d) { dir = d; };

	virtual void update();
	virtual void fireball_tombe();
};
