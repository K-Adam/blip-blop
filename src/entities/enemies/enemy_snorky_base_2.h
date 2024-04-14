#pragma once

#include "entities/enemies/enemy.h"

#define SNORKY_BASE_SPEED 2

class EnemySnorkyBase2 : public Enemy {
   public:
	int wait_for_shoot;
	int shoot_delay;
	int y_cible;
	int x_cible;

	EnemySnorkyBase2();

	virtual void update();
	virtual void onAvance();
	virtual void onTombe();
	virtual void onMeure();
	virtual void onTire();
	virtual void onCarbonise();

	virtual void estTouche(Shot* tir);
};
