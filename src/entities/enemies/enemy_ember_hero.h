#pragma once

#include "entities/enemies/enemy.h"

class EnemyEmberHero : public Enemy {
   private:
	int dx;
	int dorkeball;

   public:
	EnemyEmberHero();
	void update() override;
	virtual void onAvance();
	virtual void onMeure();
	void takeHit(Shot* shot) override;
};
