#pragma once

#include "entities/enemies/enemy.h"

class EnemySmurfGreedy : public Enemy {
   public:
	int wait_for_shoot;
	int shoot_delay;
	int xtarget;
	int ytarget;

	EnemySmurfGreedy();

	void update() override;
	virtual void onNormal();
	virtual void onShoot();
	virtual void onDie();
	virtual void onHold();
	virtual void onCharred();

	void takeHit(Shot* shot) override;
};
