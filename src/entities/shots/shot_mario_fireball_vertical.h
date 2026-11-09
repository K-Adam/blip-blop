#pragma once

#include "entities/shots/shot.h"

#define GRAVITY_FIREBALL 15
#define FIREBALL_GRAVITY_DELAY 10

class ShotMarioFireballVertical : public Shot {
   public:
	int dx;
	int x_target;

	ShotMarioFireballVertical(int vx, int xc);

	int damage() const override { return 1; };

	int enflame() const override { return 0; };

	void setDir(int d) override { dir = d; };

	void update() override;
	virtual void fireball_fall();
};
