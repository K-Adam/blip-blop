#pragma once

#include "entities/player.h"
#include "entities/shots/shot_bb.h"

class ShotBBShotgun : public ShotBB {
   public:
	int lifespan = 0;
	int age = 0;

	inline int damage() const override {
		if (age < lifespan)
			return 40;
		else
			return 30;
	};

	inline int enflame() const override { return 0; };

	// Degrees counterclockwise from the right
	void aim(double deg);
	void update() override;
	void render() override { drawAlongAngle(); }

   private:
	double drift_x_ = 0;
	double drift_y_ = 0;
	int drift_delay_ = 1;
	int drift_tick_ = 0;
};
