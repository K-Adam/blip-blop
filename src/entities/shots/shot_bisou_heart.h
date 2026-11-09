#pragma once

#include "entities/shots/shot.h"

class ShotBisouHeart : public Shot {
   public:
	int xbase;
	int phi;
	bool ok;

	ShotBisouHeart();

	int damage() const override { return 1; };

	int enflame() const override { return 0; };

	void setDir(int d) override { dir = d; };

	void update() override;
};
