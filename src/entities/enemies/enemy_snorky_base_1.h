#pragma once

#include "entities/enemies/enemy.h"

#define SNORKY_BASE_SPEED 2

class EnnemiSnorkyBase1 : public Ennemi {
   public:
	int wait_for_shoot;
	int shoot_delay;

	EnnemiSnorkyBase1();

	virtual void update();
	virtual void onAvance();
	virtual void onTombe();
	virtual void onMeure();
	virtual void onTire();
	virtual void onCarbonise();

	virtual void estTouche(Tir* tir);
};
