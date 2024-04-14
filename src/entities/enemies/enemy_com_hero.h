#pragma once

#include "entities/enemies/enemy.h"

class EnemyCOMHero : public Enemy {
   private:
	int dx;
	int dorkeball;

   public:
	EnemyCOMHero();
	virtual void update();
	virtual void onAvance();
	virtual void onMeure();
	virtual void estTouche(Shot* tir);
};
