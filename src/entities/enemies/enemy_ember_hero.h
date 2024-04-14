#pragma once

#include "entities/enemies/enemy.h"

class EnemyEmberHero : public Enemy {
   private:
	int dx;
	int dorkeball;

   public:
	EnemyEmberHero();
	virtual void update();
	virtual void onAvance();
	virtual void onMeure();
	virtual void estTouche(Shot* tir);
};
