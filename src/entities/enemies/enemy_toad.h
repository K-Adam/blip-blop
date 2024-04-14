#pragma once

#include "entities/enemies/enemy.h"

#define TOAD_SPEED 3
#define TOAD_CHARGE_SPEED 7

class EnemyToad : public Enemy {
   private:
	int speed;		  // vitesse courante
	int etape_speed;  // delai d'acceleration
	// static int toad_speed;    //vitesse maximum
	int charge_delay;
	int wait_for_charge;
	int charge_etape;
	int hokuto;

   public:
	EnemyToad();
	virtual void update();
	virtual void onAvance();
	virtual void onMeure();
	virtual void onSaute();
	virtual void onCharge();
	virtual void onCarbonise();
	virtual void estTouche(Shot* tir);
};
