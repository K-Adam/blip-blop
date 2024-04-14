#pragma once

#include "entities/enemies/enemy.h"

#define LATENCE_BOUGE 1

class EnnemiPoid : public Ennemi {
   private:
	int nb_colision;

   public:
	EnnemiPoid();
	virtual void update();
	virtual int degats();
	virtual void onMeure();
	virtual void estTouche(Tir* tir);
	virtual bool count() { return false; };
};
