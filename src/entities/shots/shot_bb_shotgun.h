#pragma once

#include "entities/player.h"
#include "entities/shots/shot_bb.h"

class TirBBFusil : public TirBB {
   public:
	TirBBFusil() : etape_dx(0), etape_dy(0) {}
	int dx;
	int ddx;
	int ddy;
	int etape_dx;
	int latence_x;
	int latence_y;
	int etape_dy;
	int duree_vie;
	int vie;

	inline virtual int degats() const {
		if (vie < duree_vie)
			return 40;
		else
			return 30;
	};

	inline virtual int enflame() const { return 0; };

	virtual void setDir(int d);
	virtual void update();
};
