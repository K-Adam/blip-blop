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

	virtual void update();
	virtual void onAvance();
	virtual void onTombe();
	virtual void onMeure();
	virtual void onTire();
	virtual void onCarbonise();

	virtual void estTouche(Shot* tir);
};
