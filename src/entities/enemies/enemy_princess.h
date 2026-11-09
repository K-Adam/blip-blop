#pragma once

#include "entities/enemies/enemy.h"

#define PRINCESS_SPEED 1

class EnemyPrincess : public Enemy {
   private:
	bool walking;
	bool guards_called;

   public:
	EnemyPrincess();
	void update() override;
	virtual void onWalk();
	virtual void onDie();

	void takeHit(Shot* shot) override;
};
