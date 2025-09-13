#pragma once

#include "entities/enemies/enemy.h"

#define SONIC_SPEED 7
#define SONIC_CHARGE_SPEED 8

class EnemySonic : public Enemy {
   private:
	int speed;
	int speed_tick;	 // Acceleration delay
	int attack_delay;
	int wait_for_attack;
	int attack_phase;
	int attack_type;
	int x_target;
	int y_target;
	int attack_frame_tick;
	int attack_frame;
	int nb_shots;
	int hp_subcritical;
	int hp_critical;
	int hp_supercritical;
	int ring;
	int wait_blood_fall;

   public:
	EnemySonic();
	void update() override;
	virtual void onWalk();
	virtual void onDie();
	virtual void onJump();
	virtual void onCharge();
	virtual void onAttackhorizontal();
	virtual void onAttackvertical();
	virtual void onAttackThorns();
	virtual void onAttackLightning();
	virtual void onThrowRings();
	virtual void throw_thorns();
	virtual void onCharred();
	void takeHit(Shot* shot) override;
};
