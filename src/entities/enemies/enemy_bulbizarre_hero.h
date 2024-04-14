#pragma once

#include "entities/enemies/enemy.h"

class EnemyBulbizarreHero : public Enemy {
   private:
	int dx;
	int dorkeball;

   public:
	EnemyBulbizarreHero();
	virtual void update();
	virtual void onAvance();
	virtual void onMeure();
	virtual void estTouche(Shot* tir);
};
