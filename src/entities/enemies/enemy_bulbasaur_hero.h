#pragma once

#include "entities/enemies/enemy.h"

class EnemyBulbasaurHero : public Enemy {
   private:
	int dx;
	int dorkeball;

   public:
	EnemyBulbasaurHero();
	void update() override;
	virtual void onWalk();
	virtual void onDie();
	void takeHit(Shot* shot) override;
};
