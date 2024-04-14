#pragma once

#include "entities/player.h"
#include "entities/shots/shot_bb.h"

class ShotBBM16 : public ShotBB {
   public:
	inline virtual int degats() const { return 40; };

	inline virtual int enflame() const { return 0; };

	virtual void update();
};
