#pragma once

#include "entities/character.h"
#include "entities/shots/shot.h"

#ifndef ENNEMI_CPP
extern Character* tete_turc;
#endif

class Enemy : public Character {
   public:
	Enemy();

	int xmin;
	int blood;
	int tresor;

	virtual bool count() { return true; };

	virtual void estTouche(Shot* tir);

	virtual int degats() { return 1; };

	virtual void gicle(const Shot* tir, const int* dxg, const int* dyg);

	void tirEnCloche(int xtir, int ytir, int xci, int yci, int& xspeed, int& yspeed) const;
};
