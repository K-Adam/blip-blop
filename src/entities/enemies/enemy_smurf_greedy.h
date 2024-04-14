#pragma once

#include "entities/enemies/enemy.h"

class EnemySmurfGreedy : public Enemy {
   public:
	int wait_for_shoot;
	int shoot_delay;
	int xcible;
	int ycible;

	EnemySmurfGreedy();

	virtual void update();
	virtual void onNormal();
	virtual void onTire();
	virtual void onMeure();
	virtual void onHold();
	virtual void onCarbonise();

	virtual void estTouche(Shot* tir);
};
