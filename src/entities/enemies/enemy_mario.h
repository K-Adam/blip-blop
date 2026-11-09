#pragma once

#include "entities/enemies/enemy.h"

#define MARIO_SPEED 5
#define MARIO_CHARGE_SPEED 7
#define MARIO_GRAVITY_DELAY 12
#define MARIO_HP_SUBCRITICAL 17500
#define MARIO_HP_CRITICAL 11500
#define MARIO_HP_SUPERCRITICAL 8500

class EnemyMario : public Enemy {
   private:
	int speed;
	int speed_tick;	 // Acceleration delay
	int fireball;	 // Fireball held in hands
	int hologram;

	int attack_delay;
	int wait_for_attack;
	int attack_step;
	int attack_tick;
	int attack_type;
	int attack_phase;
	int nb_rain;
	int x_target;
	int y_target;

   public:
	EnemyMario();
	void update() override;
	virtual void onWalk();
	virtual void onDie();
	virtual void onJump();
	virtual void onCharge();
	virtual void onCharred();
	virtual void fall_mario();

	virtual void onShootHorizontal();
	virtual void onShootVertical();
	virtual void onShootCross();

	virtual void onBurstHorizontal();
	virtual void onBurstHorizontalDouble();
	virtual void onBurstHorizontalSteady();
	virtual void onBurstHorizontalHoming();

	virtual void onBurstVertical();
	virtual void onBurstVerticalReverse();
	virtual void onBurstVerticalDouble();
	virtual void onBurstVerticalHoming();
	virtual void onBurstVerticalSteady();
	virtual void onFireRain();

	virtual void onBurstBerserker();
	virtual void onBurstCrossHoming();
	virtual void onBurstCross();

	// Fireballs only travel horizontally
	virtual inline void shoot_fireball(int velocity);

	void render() override;
	void takeHit(Shot* shot) override;
};
