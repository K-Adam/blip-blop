#pragma once

#include "enemy.h"

#define SNORKY_MAGE_SPEED 1

class EnnemiSnorkyMage : public Ennemi {
   public:
	int wait_for_shoot;
	int shoot_delay;
	int oscilation_etape;
	int concentration;

	EnnemiSnorkyMage();

	virtual void update();
	virtual void onAvance();
	virtual void onTombe();
	virtual void onMeure();
	virtual void onTire();
	virtual void onCarbonise();

	virtual void estTouche(Tir* tir);
};
