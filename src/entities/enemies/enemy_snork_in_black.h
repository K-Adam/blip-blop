#pragma once

#include "entities/enemies/enemy.h"

#define SNORK_IN_BLACK_SPEED 2
#define SNORK_IN_BLACK_MAX_SHOTS 6

class EnemySnorkInBlack : public Enemy {
   private:
	int x_target;
	int y_target;
	int nb_shots;

   public:
	int wait_for_shoot;
	int shoot_delay;

	EnemySnorkInBlack();

	void update() override;
	virtual void onWalk();
	virtual void onFall();
	virtual void onDie();
	virtual void onShoot();
	virtual void onCharred();

	void takeHit(Shot* shot) override;
};
