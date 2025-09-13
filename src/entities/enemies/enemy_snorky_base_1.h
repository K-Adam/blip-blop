#pragma once

#include "entities/enemies/enemy.h"

#define SNORKY_BASE_SPEED 2

class EnemySnorkyBase1 : public Enemy {
   public:
	int wait_for_shoot;
	int shoot_delay;

	EnemySnorkyBase1();

	void update() override;
	virtual void onWalk();
	virtual void onFall();
	virtual void onDie();
	virtual void onShoot();
	virtual void onCharred();

	void takeHit(Shot* shot) override;
};
