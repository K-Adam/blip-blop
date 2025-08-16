#pragma once

#include "entities/player.h"
#include "entities/shots/shot_bb.h"

class ShotBBCow : public ShotBB {
   public:
	inline int degats() const override { return 50; };

	inline int enflame() const override { return 2; };

	inline void aTouche(int pts) override { joueur->addPoints(pts); };

	void update() override;
};
