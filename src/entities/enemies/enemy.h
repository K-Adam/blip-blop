#pragma once

#include "entities/character.h"
#include "entities/shots/shot.h"

#ifndef ENNEMI_CPP
extern Character* targeted_player;
#endif

class Enemy : public Character {
   public:
	Enemy();

	int xmin;
	int blood;
	int tresor;

	virtual bool count() { return true; };

	virtual void takeHit(Shot* shot);

	virtual int damage() { return 1; };

	virtual void gicle(const Shot* shot, const int* dxg, const int* dyg);

	void tirEnCloche(int xtir, int ytir, int xci, int yci, int& xspeed, int& yspeed) const;
};
