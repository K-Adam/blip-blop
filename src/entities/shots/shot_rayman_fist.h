#pragma once

#include "entities/shots/shot.h"

#define FIST_DELAY 8

class ShotRaymanFist : public Shot {
   public:
	int dx;
	int x_target;
	int n_pic;

	ShotRaymanFist(int dx0, int xc);

	int damage() const override { return 1; };

	int enflame() const override { return 0; };

	void setDir(int d) override { dir = d; };

	void update() override;
};
