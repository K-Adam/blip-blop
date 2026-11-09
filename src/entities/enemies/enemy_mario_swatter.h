#pragma once

#include "entities/enemies/enemy.h"

#define MARIO_SPEED 5
#define MARIO_CHARGE_SPEED 7
#define MARIO_GRAVITY_DELAY 12
#define SHRINK_DELAY 300
#define MARIO_ENDURANCE 80
#define GROUND_RECOIL_DIVISOR 20
#define GROUND_SPEED_DIVISOR 100
#define AIR_RECOIL_DIVISOR 15
#define AIR_SPEED_DIVISOR 80

class EnemyMarioSwatter : public Enemy {
   private:
	int helping_luigi;
	int speed;
	int speed_tick;	 // Acceleration delay

	int attack_delay;
	int wait_for_attack;
	int attack_step;
	int shrink_timer;

	int shrunk;
	int knockback;

   public:
	EnemyMarioSwatter();
	void update() override;
	virtual void onWalk();
	virtual void onDie();
	virtual void onJump();
	virtual void onCharge();
	virtual void onCharred();
	virtual void fall_mario();
	void takeHit(Shot* shot) override;
};
