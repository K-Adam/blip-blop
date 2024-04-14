#pragma once

#include "entities/player.h"
#include "entities/shots/shot_bb.h"

class TirBBVache : public TirBB {
   public:
	inline virtual int degats() const { return 50; };

	inline virtual int enflame() const { return 2; };

	inline virtual void aTouche(int pts) { joueur->addPoints(pts); };

	virtual void update();
};
