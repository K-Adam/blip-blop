#pragma once

#include "entities/enemies/enemy.h"
#include "entities/enemies/enemy_princess.h"

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
