#pragma once

#include "entities/player.h"
#include "entities/shots/shot_bb.h"

class ShotBBCow : public ShotBB {
   public:
	inline int damage() const override { return 50; };

	inline int enflame() const override { return 2; };

	inline void onHit(int pts) override { hero->addPoints(pts); };

	void update() override;
};
