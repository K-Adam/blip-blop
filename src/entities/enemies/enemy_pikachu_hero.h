#pragma once

#include "entities/enemies/enemy.h"

class EnemyPikachuHero : public Enemy {
   private:
	int dx;
	int dorkeball;

   public:
	EnemyPikachuHero();
	void update() override;
	virtual void onWalk();
	virtual void onDie();
	void takeHit(Shot* shot) override;
};
