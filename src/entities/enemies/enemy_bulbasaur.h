#pragma once

#include "entities/enemies/enemy.h"

class EnemyBulbasaur : public Enemy {
   public:
	EnemyBulbasaur();
	void update() override;
	virtual void onWalk();
	virtual void onDie();
	virtual void onCharred();
	void takeHit(Shot* shot) override;
};
