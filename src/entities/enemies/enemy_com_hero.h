#pragma once

#include "entities/enemies/enemy.h"

class EnemyCOMHero : public Enemy {
   private:
	int dx;
	int dorkeball;

   public:
	EnemyCOMHero();
	void update() override;
	virtual void onWalk();
	virtual void onDie();
	void takeHit(Shot* shot) override;
};
