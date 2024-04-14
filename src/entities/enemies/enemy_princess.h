#pragma once

#include "entities/enemies/enemy.h"

#define PRINCESSE_SPEED 1

class EnnemiPrincesse : public Ennemi {
   private:
	bool avance;
	bool aide;

   public:
	EnnemiPrincesse();
	virtual void update();
	virtual void onAvance();
	virtual void onMeure();

	virtual void estTouche(Tir* tir);
};
