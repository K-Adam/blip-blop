#pragma once

#include "entities/player.h"
#include "entities/shots/shot_bb.h"

class ShotBBPM : public ShotBB {
   public:
	inline int damage() const override { return 75; };

	inline int enflame() const override { return 0; };

	void update() override;
};
