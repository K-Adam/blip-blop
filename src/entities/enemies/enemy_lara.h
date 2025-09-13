#pragma once

#include "entities/enemies/enemy.h"

#define LARA_SHOT_SPEED 10
#define ATTACK_DELAY 10

class EnemyLara : public Enemy {
   private:
	int bust_pic_num;
	int attack_delay;
	int wait_for_attack;
	int attack_step;
	int attack_tick;
	int attack_type;
	int attack_phase;
	int nb_rain;
	int x_target;
	int y_target;
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
