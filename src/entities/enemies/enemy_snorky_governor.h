#pragma once

#include "entities/enemies/enemy.h"

#define GOUVERNEUR_SPEED 1

class EnemySnorkyGovernor : public Enemy {
   public:
	int nb_tir;
	int wait_for_shoot;
	int shoot_delay;

	EnemySnorkyGovernor();

	virtual void update();
	virtual void onAvance();
	virtual void onTombe();
	virtual void onMeure();
	virtual void onTire();
	virtual void onCarbonise();

	virtual void estTouche(Shot* tir);
};
