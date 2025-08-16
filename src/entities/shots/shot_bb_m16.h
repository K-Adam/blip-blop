#pragma once

#include "entities/player.h"
#include "entities/shots/shot_bb.h"

class ShotBBM16 : public ShotBB {
   public:
	inline int degats() const override { return 40; };

	inline int enflame() const override { return 0; };

	void update() override;
};
