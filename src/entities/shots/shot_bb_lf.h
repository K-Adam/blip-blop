#pragma once

#include "entities/player.h"
#include "entities/shots/shot_bb.h"

class ShotBBLF : public ShotBB {
   public:
	int type;
	int dx;
	int delay;
	bool finished;
	int speed;
	int deg;

	void setDir(int d) override;

	inline int damage() const override { return deg; };

	inline int enflame() const override { return 2; };

	void update() override;

	void onHit(int pts) override;
};
