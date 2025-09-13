#pragma once

#include "entities/enemies/enemy.h"

class EnemyEmberHero : public Enemy {
   private:
	int dx;
	int dorkeball;

   public:
	EnemyEmberHero();
	void update() override;
	virtual void onWalk();
	virtual void onDie();
	void takeHit(Shot* shot) override;
};
