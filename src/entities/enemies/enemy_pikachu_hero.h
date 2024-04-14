#pragma once

#include "entities/enemies/enemy.h"

class EnemyPikachuHero : public Enemy {
   private:
	int dx;
	int dorkeball;

   public:
	EnemyPikachuHero();
	virtual void update();
	virtual void onAvance();
	virtual void onMeure();
	virtual void estTouche(Shot* tir);
};
