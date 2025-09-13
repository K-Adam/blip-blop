#pragma once

#include "entities/enemies/enemy.h"

#define MARIO_SPEED 5
#define MARIO_GRAVITY_DELAY 12
#define NB_BLINKS 4

class EnemyMarioHologram : public Enemy {
   private:
	int speed;
	int speed_tick;	 // Acceleration delay
	int fireball;	 // Fireball held in hands
	int wait_for_blink;
	int blink_delay;
	int blink_offset;

	int attack_delay;
	int wait_for_attack;
	int attack_step;
	int attack_type;
	int attack_phase;
	int x_target;
	int y_target;

   public:
	EnemyMarioHologram();
	void update() override;
	virtual void onWalk();
	virtual void onDie();
	virtual void onJump();
	virtual void onCharge();
	virtual void onCharred();
	virtual void onShoot();
	virtual void onShootVertical();
	virtual void fall_mario();
	virtual inline void shoot_fireball();
	void render() override;
	void takeHit(Shot* shot) override;
};
