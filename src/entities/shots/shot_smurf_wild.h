#pragma once

#include "entities/shots/shot.h"

class ShotSmurfWild : public Shot {
   public:
	int dx;
	int dy;

	ShotSmurfWild();
	int degats() const override { return 1; };

	int enflame() const override { return 0; };

	void setDir(int d) override { dir = d; };

	void update() override;
};
