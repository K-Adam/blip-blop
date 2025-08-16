#pragma once

#include "entities/player.h"
#include "entities/shots/shot_bb.h"

class ShotBBLF : public ShotBB {
   public:
	int type;
	int dx;
	int latence;
	bool fini;
	int speed;
	int deg;

	void setDir(int d) override;

	inline int degats() const override { return deg; };

	inline int enflame() const override { return 2; };

	void update() override;

	void aTouche(int pts) override;
};
