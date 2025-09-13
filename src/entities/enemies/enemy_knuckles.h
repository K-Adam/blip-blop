#pragma once

#include "entities/enemies/enemy.h"

#define KNUCKLES_SPEED 3
#define KNUCKLES_CHARGE_SPEED 6
#define KNUCKLES_JUMP_SPEED 12
#define KNUCKLES_GRAVITY_MAX 7

class EnemyKnuckles : public Enemy {
   private:
	int speed;
	int speed_tick;	 // Acceleration delay
	int charge_delay;
	int wait_for_charge;

   public:
	EnemyKnuckles();
	void update() override;
	virtual void onWalk();
	virtual void onDie();
	virtual void onFall();
	virtual void onCharred();
	virtual void onCharge();
	virtual void onJump();
	void takeHit(Shot* shot) override;
	void fall() override;
};
