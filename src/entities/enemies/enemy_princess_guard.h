#pragma once

#include "entities/enemies/enemy.h"
#include "entities/enemies/enemy_princess.h"

#define GUARD_SPEED 1

class EnemyPrincessGuard : public Enemy {
   private:
	int cover_distance;

   public:
	EnemyPrincess* princess;

	EnemyPrincessGuard(EnemyPrincess* Princess, int distance);
	void update() override;
	virtual void onWalk();
	virtual void onDie();
	void takeHit(Shot* shot) override;
};
