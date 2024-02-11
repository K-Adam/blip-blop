#pragma once

#include "enemy.h"
#include "ennemi_princesse.h"

#define GUARD_SPEED 1

class EnnemiPrincesseguard : public Ennemi {
   private:
	int distance_couverture;

   public:
	EnnemiPrincesse* princesse;

	EnnemiPrincesseguard(EnnemiPrincesse* Princesse, int distance);
	virtual void update();
	virtual void onAvance();
	virtual void onMeure();
	virtual void estTouche(Tir* tir);
};
