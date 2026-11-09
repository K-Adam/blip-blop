#pragma once

#include "entities/enemies/enemy.h"

#define SNORKY_BASE_SPEED 2

class EnemySnorkyBase2 : public Enemy {
   public:
	int wait_for_shoot;
	int shoot_delay;
	int y_target;
	int x_target;

	EnemySnorkyBase2();

	void update() override;
	virtual void onWalk();
	virtual void onFall();
	virtual void onDie();
	virtual void onShoot();
	virtual void onCharred();

	void takeHit(Shot* shot) override;
};
