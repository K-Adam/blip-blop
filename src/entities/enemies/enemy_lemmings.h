#pragma once

#include "entities/enemies/enemy.h"

#define LEM_SPEED 1

class EnemyLemmings : public Enemy {
   public:
	bool suicide;

	EnemyLemmings();

	void update() override;
	virtual void onWalk();
	virtual void onFall();
	virtual void onDie();
	virtual void onCharred();

	void takeHit(Shot* shot) override;
};
