#pragma once

#include "entities/enemies/enemy.h"

class EnemySmurfStork : public Enemy {
   public:
	int base_y;
	int base_x;
	bool first;
	int theta;

	EnemySmurfStork();

	void update() override;
};
