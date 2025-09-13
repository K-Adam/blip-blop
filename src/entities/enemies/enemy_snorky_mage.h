#pragma once

#include "entities/enemies/enemy.h"

#define SNORKY_MAGE_SPEED 1

class EnemySnorkyMage : public Enemy {
   public:
	int wait_for_shoot;
	int shoot_delay;
	int oscilation_etape;
	int concentration;

	EnemySnorkyMage();

	void update() override;
	virtual void onAvance();
	virtual void onTombe();
	virtual void onMeure();
	virtual void onTire();
	virtual void onCarbonise();

	void takeHit(Shot* shot) override;
};
