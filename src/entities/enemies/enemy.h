#pragma once

#include "entities/character.h"
#include "entities/shots/shot.h"

class Enemy : public Character {
   public:
	Enemy();

	int xmin;
	int blood;
	int treasure;

	virtual bool count() { return true; };

	virtual void takeHit(Shot* shot);

	virtual int damage() { return 1; };

	virtual void splatter(const Shot* shot, const int* dxg, const int* dyg);

	void lobShot(int xshot, int yshot, int xdest, int ydest, int& xspeed, int& yspeed) const;
};
