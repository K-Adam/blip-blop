#pragma once

#include "entities/shots/shot.h"

class ShotCake : public Shot {
   public:
	int dx;

	ShotCake();

	int degats() const override { return 1; };

	int enflame() const override { return 0; };

	void setDir(int d) override { dir = d; };

	void update() override;
};
