#pragma once

#include "entities/enemies/enemy.h"

class EnemyBisouStar : public Enemy {
   public:
	int wait_shoot;
	int shoot_step;

	EnemyBisouStar();
	virtual void onWalk();
	virtual void onShoot();
	virtual void onDieWhole();
	virtual void onDieTop();
	virtual void onDieTopEnd();
	virtual void onCharred();
	void takeHit(Shot* shot) override;
	void update() override;
};
