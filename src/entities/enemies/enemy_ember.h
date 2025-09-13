#pragma once

#include "entities/enemies/enemy.h"

class EnemyEmber : public Enemy {
   public:
	EnemyEmber();
	void update() override;
	virtual void onWalk();
	virtual void onDie();
	void takeHit(Shot* shot) override;
};
