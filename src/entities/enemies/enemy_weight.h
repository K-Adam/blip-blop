#pragma once

#include "entities/enemies/enemy.h"

#define HITS_PER_MOVE 1

class EnemyWeight : public Enemy {
   private:
	int nb_collisions;

   public:
	EnemyWeight();
	void update() override;
	int damage() override;
	virtual void onDie();
	void takeHit(Shot* shot) override;
	bool count() override { return false; };
};
