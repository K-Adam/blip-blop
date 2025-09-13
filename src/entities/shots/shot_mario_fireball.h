#pragma once

#include "entities/shots/shot.h"

#define DELAY_ACCELERATION 8

class ShotMarioFireball : public Shot {
   public:
	int speed_etape;
	int dx;

	ShotMarioFireball(int vx);

	int damage() const override { return 1; };

	int enflame() const override { return 0; };

	void setDir(int d) override { dir = d; };

	void update() override;
};
