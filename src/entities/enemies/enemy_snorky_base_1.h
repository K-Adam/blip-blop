#pragma once

#include "entities/enemies/enemy.h"

#define SNORKY_BASE_SPEED 2

class EnemySnorkyBase1 : public Enemy {
   public:
	int wait_for_shoot;
	int shoot_delay;

	EnemySnorkyBase1();

	virtual void update();
	virtual void onAvance();
	virtual void onTombe();
	virtual void onMeure();
	virtual void onTire();
	virtual void onCarbonise();

	virtual void estTouche(Shot* tir);
};
