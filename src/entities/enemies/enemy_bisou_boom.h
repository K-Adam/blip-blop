#pragma once

#include "entities/enemies/enemy.h"

class EnemyBisouBoom : public Enemy {
   public:
	EnemyBisouBoom();
	virtual void onWalk();
	virtual void onDie();
	void takeHit(Shot* shot) override;
	void update() override;
};
