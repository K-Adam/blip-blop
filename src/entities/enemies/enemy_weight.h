#pragma once

#include "entities/enemies/enemy.h"

#define LATENCE_BOUGE 1

class EnemyWeight : public Enemy {
   private:
	int nb_colision;

   public:
	EnemyWeight();
	virtual void update();
	virtual int degats();
	virtual void onMeure();
	virtual void estTouche(Shot* tir);
	virtual bool count() { return false; };
};
