#pragma once

#include "entities/enemies/enemy.h"
#include "entities/shots/shot_rayman_fist.h"

#define RAYMAN_SPEED 3
#define RAYMAN_CRITICAL_SPEED 6
#define RAYMAN_FIST1_SPEED 7
#define RAYMAN_FIST2_SPEED 10
#define HP_SUBCRITICAL 10000
#define HP_CRITICAL 5000
#define HP_SUPERCRITICAL 1000

class EnemyRayman : public Enemy {
   private:
	int attack_delay;
	int wait_for_attack;
	int attack_step;
	int x_target;
	int fist_range;
	int fist1;
	int fist2;
	int feet;
	int dorkemon;

	ShotRaymanFist* fist_shot;

   public:
	EnemyRayman();
	void update() override;
	virtual void onWalk();
	virtual void onDie();
	virtual void onFall();
	virtual void onShoot();
	void takeHit(Shot* shot) override;
};
