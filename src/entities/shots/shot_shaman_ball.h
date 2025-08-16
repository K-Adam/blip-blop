#pragma once

#include "entities/shots/shot.h"

class ShotShamanBall : public Shot {
   public:
	int dx;
	bool ok;
	bool boom;

	ShotShamanBall();

	int degats() const override { return 1; };

	int enflame() const override { return 0; };

	void setDir(int d) override { dir = d; };

	void update() override;
};
