#pragma once

#include "entities/enemies/enemy.h"

class EnemyBisouHeart : public Enemy {
   public:
	EnemyBisouHeart();
	virtual void onWalk();
	virtual void onDie();
	virtual void onLeave();
	void takeHit(Shot* shot) override;
	void update() override;
};
