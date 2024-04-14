#pragma once

#include "entities/enemies/enemy.h"

class EnnemiPacman : public Ennemi {
   private:
	int tir;
	int etape_tir;
	int ss_etape_tir;

	int dx;

   public:
	EnnemiPacman();
	virtual void update();
	virtual int degats();
	virtual void affiche();
	virtual void estTouche(Tir* tir);

	virtual bool count() { return false; };
};
