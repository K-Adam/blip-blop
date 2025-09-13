#pragma once

#include "entities/enemies/enemy.h"

#define TOAD_SPEED 3
#define TOAD_CHARGE_SPEED 7

class EnemyToad : public Enemy {
   private:
	int speed;
	int speed_tick;	 // Acceleration delay
	int charge_delay;
	int wait_for_charge;
	int charge_step;
	int hokuto;

   public:
	EnemyToad();
	void update() override;
	virtual void onWalk();
	virtual void onDie();
	virtual void onJump();
	virtual void onCharge();
	virtual void onCharred();
	void takeHit(Shot* shot) override;
};
