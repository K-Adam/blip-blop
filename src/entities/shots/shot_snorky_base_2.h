#pragma once

#include "entities/shots/shot.h"

class ShotSnorkyBase2 : public Shot {
   public:
	int dx;

	ShotSnorkyBase2(int vx, int vy);

	int degats() const override { return 1; };

	int enflame() const override { return 0; };

	void setDir(int d) override { dir = d; };

	void update() override;
};
