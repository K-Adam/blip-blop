#pragma once

#include "entities/enemies/enemy.h"

#define LEM_SPEED 1

class EnemyLemmings : public Enemy {
   public:
	bool suicide;

	EnemyLemmings();

	virtual void update();
	virtual void onAvance();
	virtual void onTombe();
	virtual void onMeure();
	virtual void onCarbonise();

	virtual void estTouche(Shot* tir);
};
