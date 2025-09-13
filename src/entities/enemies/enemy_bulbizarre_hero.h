#pragma once

#include "entities/enemies/enemy.h"

class EnemyBulbizarreHero : public Enemy {
   private:
	int dx;
	int dorkeball;

   public:
	EnemyBulbizarreHero();
	void update() override;
	virtual void onAvance();
	virtual void onMeure();
	void takeHit(Shot* shot) override;
};
