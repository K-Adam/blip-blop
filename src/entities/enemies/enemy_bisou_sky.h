#pragma once

#include "entities/enemies/enemy.h"

class EnemyBisouSky : public Enemy {
   public:
	int wait_for_pee;

	EnemyBisouSky();
	virtual void onWalk();
	virtual void onShoot();
	virtual void onDieWhole();
	virtual void onDieTop();
	virtual void onDieTopEnd();
	virtual void onCharred();
	void takeHit(Shot* shot) override;
	void update() override;
};
