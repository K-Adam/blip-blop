#pragma once

#include "entities/shots/shot.h"

class ShotThorn : public Shot {
   public:
	int dx;
	bool traverse;

	ShotThorn(int n_pbk_ennemis, int vx, int vy);

	int damage() const override { return 1; };

	int enflame() const override { return 0; };

	void setDir(int d) override { dir = d; };

	void update() override;
};
