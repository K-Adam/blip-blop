#pragma once

#include "entities/player.h"
#include "entities/shots/shot_bb.h"

class ShotBBShotgun : public ShotBB {
   public:
	ShotBBShotgun() : dx_tick(0), dy_tick(0) {}
	int dx;
	int ddx;
	int ddy;
	int dx_tick;
	int delay_x;
	int delay_y;
	int dy_tick;
	int lifespan;
	int age;

	inline int damage() const override {
		if (age < lifespan)
			return 40;
		else
			return 30;
	};

	inline int enflame() const override { return 0; };

	void setDir(int d) override;
	void update() override;
};
