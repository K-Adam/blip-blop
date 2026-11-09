#pragma once

#include "entities/shots/shot.h"

class ShotLaserSword : public Shot {
   public:
	Sprite* target;
	Sprite* thrower;
	bool isdead;
	bool returning;
	int dx;
	int lifespan;
	int accel;

	ShotLaserSword();
	virtual ~ShotLaserSword();

	int damage() const override { return 1; };

	int enflame() const override { return 0; };

	void setDir(int d) override { dir = d; };

	void update() override;
};
