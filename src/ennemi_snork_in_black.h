#pragma once

#include "enemy.h"

#define SNORK_IN_BLACK_SPEED 2
#define SNORK_IN_BLACK_MAX_TIR 6

class EnnemiSnorkInBlack : public Ennemi {
   private:
	int x_cible;
	int y_cible;
	int nb_tir;

   public:
	int wait_for_shoot;
	int shoot_delay;

	EnnemiSnorkInBlack();

	virtual void update();
	virtual void onAvance();
	virtual void onTombe();
	virtual void onMeure();
	virtual void onTire();
	virtual void onCarbonise();

	virtual void estTouche(Tir* tir);
};
