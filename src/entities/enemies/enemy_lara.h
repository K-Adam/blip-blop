#pragma once

#include "entities/enemies/enemy.h"

#define LARA_SHOT_SPEED 10
#define ATTACK_DELAY 10

class EnemyLara : public Enemy {
   private:
	int bust_pic_num;
	int wait_for_attack;
	int pics;
	int rayman_head;

   public:
	EnemyLara();
	void update() override;
	virtual void onWalk();
	virtual void onDie();
	virtual void onShoot();

	void render() override;
	void takeHit(Shot* shot) override;
};
