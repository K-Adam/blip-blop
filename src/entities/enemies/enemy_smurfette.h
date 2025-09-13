#pragma once

#include "entities/enemies/enemy.h"

class EnemySmurfette : public Enemy {
   public:
	int shoot_step;
	int wait_shoot;

	EnemySmurfette();

	void update() override;
	virtual void onWalk();
	virtual void onShoot();
	void takeHit(Shot* shot) override;
	virtual void onCharred();
	virtual void onDie();
};
