#pragma once

#include "entities/enemies/enemy.h"

class EnemyYoshiAntiair : public Enemy {
   private:
	int attack_delay;
	int wait_for_attack;

   public:
	EnemyYoshiAntiair();

	void update() override;
	virtual void onWalk();
	virtual void onDie();
	virtual void onShoot();
	virtual void onCharred();

	void takeHit(Shot* shot) override;
};
