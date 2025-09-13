#pragma once

#include "entities/enemies/enemy.h"

#define HYPORIDER_SPEED 1
#define HYPORIDER_CHARGE_SPEED 8

class EnemySnorkyHyporider : public Enemy {
   public:
	int wait_for_charge;
	int charge_delay;
	int nageoire;
	int attack_step;

	EnemySnorkyHyporider();

	void update() override;
	virtual void onAvance();
	virtual void onMeure();
	virtual void onCharge();
	void render() override;
	virtual void onCarbonise();

	void takeHit(Shot* shot) override;
};
