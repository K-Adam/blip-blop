#pragma once

#include "entities/enemies/enemy.h"

class EnemySmurfStork : public Enemy {
   public:
	int base_y;
	int base_x;
	bool first;
	int teta;

	EnemySmurfStork();

	virtual void update();
};
