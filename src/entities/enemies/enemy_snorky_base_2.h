#pragma once

#include "entities/enemies/enemy.h"

#define SNORKY_BASE_SPEED 2

class EnnemiSnorkyBase2 : public Ennemi {
   public:
	int wait_for_shoot;
	int shoot_delay;
	int y_cible;
	int x_cible;

	EnnemiSnorkyBase2();

	virtual void update();
	virtual void onAvance();
	virtual void onTombe();
	virtual void onMeure();
	virtual void onTire();
	virtual void onCarbonise();

	virtual void estTouche(Tir* tir);
};
