#pragma once

#include "entities/enemies/enemy.h"

#define tolerance 5

class EnemySnorkInBlackBoss : public Enemy {
   private:
	int nb_shots;
	int position;
	int type_attack;
	int dx;
	bool attack;

	int x_shark;
	int y_shark;
	bool shark;
	bool jump;

   public:
	EnemySnorkInBlackBoss();

	void update() override;
	virtual void onWalk();
	virtual void onFall();
	virtual void onDie();
	virtual void onShoot();
	void render() override;

	void takeHit(Shot* shot) override;
};
