#pragma once

#include "entities/enemies/enemy.h"

#define YOSHI_SPEED 3

class EnemyYoshi : public Enemy {
   private:
	bool attack;  // False: shell attack, true: tongue contact
	int attack_delay;
	int wait_for_attack;

   public:
	EnemyYoshi();

	void update() override;
	virtual void onWalk();
	virtual void onFall();
	virtual void onDie();
	virtual void onMelee();
	virtual void onShoot();
	virtual void onCharred();

	void takeHit(Shot* shot) override;
};
