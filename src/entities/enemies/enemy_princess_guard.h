#pragma once

#include "entities/enemies/enemy.h"
#include "entities/enemies/enemy_princess.h"

#define GUARD_SPEED 1

class EnemyPrincessGuard : public Enemy {
   private:
	int distance_couverture;

   public:
	EnemyPrincess* princesse;

	EnemyPrincessGuard(EnemyPrincess* Princesse, int distance);
	void update() override;
	virtual void onAvance();
	virtual void onMeure();
	void takeHit(Shot* shot) override;
};
