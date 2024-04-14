#pragma once

#include "entities/enemies/enemy.h"

class EnemyBisouBoom : public Enemy {
   public:
	EnemyBisouBoom();
	virtual void onAvance();
	virtual void onMeure();
	virtual void estTouche(Shot* tir);
	virtual void update();
};
