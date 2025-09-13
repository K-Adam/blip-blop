#pragma once

#include "entities/enemies/enemy.h"

class EnemyBulbizarreHero : public Enemy {
   private:
	int dx;
	int dorkeball;

   public:
	EnemyBulbizarreHero();
	void update() override;
	virtual void onWalk();
	virtual void onDie();
	void takeHit(Shot* shot) override;
};
