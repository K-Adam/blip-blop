#pragma once

#include "entities/enemies/enemy.h"

class EnemyBisouSIB : public Enemy {
   public:
	int hits_taken;

	EnemyBisouSIB();

	void update() override;
	void takeHit(Shot* shot) override;
};
