#pragma once

#include "entities/enemies/enemy.h"

#define PRINCESSE_SPEED 1

class EnemyPrincess : public Enemy {
   private:
	bool avance;
	bool aide;

   public:
	EnemyPrincess();
	virtual void update();
	virtual void onAvance();
	virtual void onMeure();

	virtual void estTouche(Shot* tir);
};
