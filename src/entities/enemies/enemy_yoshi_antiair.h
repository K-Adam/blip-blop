#pragma once

#include "entities/enemies/enemy.h"

class EnemyYoshiAntiair : public Enemy {
   private:
	int attack_delay;
	int wait_for_attack;

   public:
	EnemyYoshiAntiair();

	virtual void update();
	virtual void onAvance();
	virtual void onMeure();
	virtual void onTire();
	virtual void onCarbonise();

	virtual void estTouche(Shot* tir);
};
