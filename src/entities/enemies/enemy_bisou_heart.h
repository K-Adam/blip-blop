#pragma once

#include "entities/enemies/enemy.h"

class EnemyBisouHeart : public Enemy {
   public:
	EnemyBisouHeart();
	virtual void onAvance();
	virtual void onMeure();
	virtual void onPart();
	virtual void estTouche(Shot* tir);
	virtual void update();
};
