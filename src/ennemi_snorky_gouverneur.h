#pragma once

#include "enemy.h"

#define GOUVERNEUR_SPEED 1

class EnnemiSnorkyGouverneur : public Ennemi {
   public:
	int nb_tir;
	int wait_for_shoot;
	int shoot_delay;

	EnnemiSnorkyGouverneur();

	virtual void update();
	virtual void onAvance();
	virtual void onTombe();
	virtual void onMeure();
	virtual void onTire();
	virtual void onCarbonise();

	virtual void estTouche(Tir* tir);
};
