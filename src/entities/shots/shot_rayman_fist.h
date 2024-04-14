#pragma once

#include "entities/shots/shot.h"

#define LATENCE_POING 8

class ShotRaymanFist : public Shot {
   public:
	int dx;
	int x_cible;
	int n_pic;

	ShotRaymanFist(int dx0, int xc);

	virtual int degats() const { return 1; };

	virtual int enflame() const { return 0; };

	virtual void setDir(int d) { dir = d; };

	virtual void update();
};
