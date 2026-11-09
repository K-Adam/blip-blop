#pragma once

#include "entities/enemies/enemy.h"

#define LUIGI_SPEED 6
#define LUIGI_GRAVITY_DELAY 12
#define EXPLODE_DELAY 180
#define NB_SPLATTERS 90

#define GROUND_RECOIL_DIVISOR 20
#define GROUND_SPEED_DIVISOR 100
#define AIR_RECOIL_DIVISOR 15
#define AIR_SPEED_DIVISOR 80

class EnemyLuigi : public Enemy {
   private:
	int speed;
	int speed_tick;	 // Acceleration delay

	int attack_delay;
	int wait_for_attack;
	int attack_type;
	int attack_frame_tick;
	int attack_frame;
	int nb_shots;
	int knockback;

   public:
	EnemyLuigi();
	void update() override;
	virtual void onWalk();
	virtual void onDie();
	virtual void onJump();
	virtual void onAttack();
	virtual void onCharred();
	virtual void fall_luigi();
	void takeHit(Shot* shot) override;
};
