#pragma once

#include "entities/enemies/enemy.h"

#define SNORKY_BASE_SPEED 2

class EnemySnorkyBase1 : public Enemy {
   public:
	int wait_for_shoot;
	int shoot_delay;

	EnemySnorkyBase1();

	void update() override;
	virtual void onAvance();
	virtual void onTombe();
	virtual void onMeure();
	virtual void onTire();
	virtual void onCarbonise();

	void takeHit(Shot* shot) override;
};
