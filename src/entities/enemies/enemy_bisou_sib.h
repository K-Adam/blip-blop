#pragma once

#include "entities/enemies/enemy.h"

class EnemyBisouSIB : public Enemy {
   public:
	int encaisse;

	EnemyBisouSIB();

	virtual void update();
	virtual void estTouche(Shot* tir);
};
