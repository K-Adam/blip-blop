#pragma once

#include "entities/enemies/enemy.h"

#define SNORKY_MAGE_SPEED 1

class EnemySnorkyMage : public Enemy {
   public:
	int wait_for_shoot;
	int shoot_delay;
	int oscillation_step;
	int concentration;

	EnemySnorkyMage();

	void update() override;
	virtual void onWalk();
	virtual void onFall();
	virtual void onDie();
	virtual void onShoot();
	virtual void onCharred();

	void takeHit(Shot* shot) override;
};
