#pragma once

#include "entities/shots/shot.h"

#define GRAVITE_FIREBALL 15
#define LATENCE_FIREBALL 10

class ShotMarioFireballVertical : public Shot {
   public:
	int dx;
	int x_cible;

	ShotMarioFireballVertical(int vx, int xc);

	int degats() const override { return 1; };

	int enflame() const override { return 0; };

	void setDir(int d) override { dir = d; };

	void update() override;
	virtual void fireball_tombe();
};
