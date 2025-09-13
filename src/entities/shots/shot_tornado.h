#pragma once

#include "entities/shots/shot.h"

#define SPEED_MAX 7
#define TORNADO_ACCELERATION 10
#define LOOPS_MAX 1
#define TORNADO_GRAVITY_DELAY 21

class ShotTornado : public Shot {
   public:
	int dx;
	int speed_tick;
	int nb_loops;

	ShotTornado(int vx);

	int damage() const override { return 1; };

	int enflame() const override { return 0; };

	void setDir(int d) override { dir = d; };

	void update() override;
};
