#pragma once

#include "entities/enemies/enemy.h"

#define TAILS_SPEED 3
#define TAILS_FLY_SPEED 6

class EnemyTails : public Enemy {
   private:
	int speed;
	int speed_tick;	 // Acceleration delay
	int fly_delay;
	int wait_for_fly;
	int x_target;
	int y_target;
	bool attack;  // Attacking, otherwise moving to the right height
	bool fly;
	int direction;

   public:
	EnemyTails();
	void update() override;
	virtual void onWalk();
	virtual void onDie();
	virtual void onFall();
	virtual void onFly();
	virtual void onCharred();
	void takeHit(Shot* shot) override;
};
