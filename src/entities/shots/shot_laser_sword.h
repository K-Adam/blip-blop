#pragma once

#include "entities/shots/shot.h"

class ShotLaserSword : public Shot {
   public:
	Sprite* cible;
	Sprite* lanceur;
	bool isdead;
	bool retour;
	int dx;
	int duree_vie;
	int accel;

	ShotLaserSword();
	virtual ~ShotLaserSword();

	int degats() const override { return 1; };

	int enflame() const override { return 0; };

	void setDir(int d) override { dir = d; };

	void update() override;
};
