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

	virtual int degats() const { return 1; };

	virtual int enflame() const { return 0; };

	virtual void setDir(int d) { dir = d; };

	virtual void update();
};
