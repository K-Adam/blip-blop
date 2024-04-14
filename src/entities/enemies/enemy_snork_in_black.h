#pragma once

#include "entities/enemies/enemy.h"

#define SNORK_IN_BLACK_SPEED 2
#define SNORK_IN_BLACK_MAX_TIR 6

class EnemySnorkInBlack : public Enemy {
   private:
	int x_cible;
	int y_cible;
	int nb_tir;

   public:
	int wait_for_shoot;
	int shoot_delay;

	EnemySnorkInBlack();

	virtual void update();
	virtual void onAvance();
	virtual void onTombe();
	virtual void onMeure();
	virtual void onTire();
	virtual void onCarbonise();

	virtual void estTouche(Shot* tir);
};
