#pragma once

#include "entities/enemies/enemy.h"

#define GOVERNOR_SPEED 1

class EnemySnorkyGovernor : public Enemy {
   public:
	int nb_shots;
	int wait_for_shoot;
	int shoot_delay;

	EnemySnorkyGovernor();

	void update() override;
	virtual void onWalk();
	virtual void onFall();
	virtual void onDie();
	virtual void onShoot();
	virtual void onCharred();

	void takeHit(Shot* shot) override;
};
