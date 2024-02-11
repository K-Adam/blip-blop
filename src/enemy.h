#pragma once

#include "personnage.h"
#include "tir.h"

#ifndef ENNEMI_CPP
extern Personnage* tete_turc;
#endif

class Ennemi : public Personnage {
   public:
	Ennemi();

	int xmin;
	int blood;
	int tresor;

	virtual bool count() { return true; };

	virtual void estTouche(Tir* tir);

	virtual int degats() { return 1; };

	virtual void gicle(const Tir* tir, const int* dxg, const int* dyg);

	void tirEnCloche(int xtir, int ytir, int xci, int yci, int& xspeed, int& yspeed) const;
};
