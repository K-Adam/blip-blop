#pragma once

#include "entities/enemies/enemy.h"

#define HYPORIDER_SPEED 1
#define HYPORIDER_CHARGE_SPEED 8

class EnemySnorkyHyporider : public Enemy {
   public:
	int wait_for_charge;
	int charge_delay;
	int nageoire;
	int attack_etape;

	EnemySnorkyHyporider();

	virtual void update();
	virtual void onAvance();
	virtual void onMeure();
	virtual void onCharge();
	virtual void affiche();
	virtual void onCarbonise();

	virtual void estTouche(Shot* tir);
};
