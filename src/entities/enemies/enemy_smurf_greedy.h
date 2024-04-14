#pragma once

#include "entities/enemies/enemy.h"

class EnnemiSmurfGourmand : public Ennemi {
   public:
	int wait_for_shoot;
	int shoot_delay;
	int xcible;
	int ycible;

	EnnemiSmurfGourmand();

	virtual void update();
	virtual void onNormal();
	virtual void onTire();
	virtual void onMeure();
	virtual void onHold();
	virtual void onCarbonise();

	virtual void estTouche(Tir* tir);
};
