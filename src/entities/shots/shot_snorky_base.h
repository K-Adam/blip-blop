#pragma once

#include "entities/shots/shot.h"

#define DELAY_ACCELERATION 20

class ShotSnorkyBase : public Shot {
   public:
	int oscillation_step;
	int accel_tick;
	int dx;

	ShotSnorkyBase(int vx);

	int damage() const override { return 1; };

	int enflame() const override { return 0; };

	void setDir(int d) override { dir = d; };

	void update() override;
};
