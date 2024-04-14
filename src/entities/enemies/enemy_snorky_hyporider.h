#pragma once

#include "entities/enemies/enemy.h"

#define HYPORIDER_SPEED 1
#define HYPORIDER_CHARGE_SPEED 8

class EnnemiSnorkyHyporider : public Ennemi {
   public:
	int wait_for_charge;
	int charge_delay;
	int nageoire;
	int attack_etape;

	EnnemiSnorkyHyporider();

	virtual void update();
	virtual void onAvance();
	virtual void onMeure();
	virtual void onCharge();
	virtual void affiche();
	virtual void onCarbonise();

	virtual void estTouche(Tir* tir);
};
