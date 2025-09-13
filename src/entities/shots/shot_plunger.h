#pragma once

#include "entities/shots/shot.h"

#define PLUNGER_SPEED 10

class ShotPlunger : public Shot {
   public:
	int dx;

	ShotPlunger(int n_pbk_enemies, int vx, int vy);

	int damage() const override { return 1; };

	int enflame() const override { return 0; };

	void setDir(int d) override { dir = d; };

	void update() override;
};
