#pragma once

#include "entities/shots/shot.h"

#define DEBOUCHE_CHIOTE_SPEED 10

class ShotPlunger : public Shot {
   public:
	int dx;

	ShotPlunger(int n_pbk_ennemis, int vx, int vy);

	virtual int degats() const { return 1; };

	virtual int enflame() const { return 0; };

	virtual void setDir(int d) { dir = d; };

	virtual void update();
};
