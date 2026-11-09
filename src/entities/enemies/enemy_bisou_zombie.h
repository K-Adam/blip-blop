#pragma once

#include "entities/enemies/enemy.h"

class EnemyBisouZombie : public Enemy {
   public:
	bool my_phase;
	bool my_phase2;
	int anim_step2;

	EnemyBisouZombie();
	virtual void onSevered();
	virtual void onWalk();
	virtual void onFall();
	virtual void onCrawl();
	virtual void onDie();
	virtual void onCharred();
	virtual void onCharredTop();
	void takeHit(Shot* shot) override;
	void update() override;
};
