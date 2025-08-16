#pragma once

#include "entities/player.h"
#include "entities/shots/shot_bb.h"

class ShotBBShotgun : public ShotBB {
   public:
	ShotBBShotgun() : etape_dx(0), etape_dy(0) {}
	int dx;
	int ddx;
	int ddy;
	int etape_dx;
	int latence_x;
	int latence_y;
	int etape_dy;
	int duree_vie;
	int vie;

	inline int degats() const override {
		if (vie < duree_vie)
			return 40;
		else
			return 30;
	};

	inline int enflame() const override { return 0; };

	void setDir(int d) override;
	void update() override;
};
