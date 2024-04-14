#pragma once

#include "entities/enemies/enemy.h"

class EnemyEmber : public Enemy {
   public:
	EnemyEmber();
	virtual void update();
	virtual void onAvance();
	virtual void onMeure();
	virtual void estTouche(Shot* tir);
};
