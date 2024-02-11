#pragma once

#include "enemy.h"

class EnnemiYoshiDCA : public Ennemi {
   private:
	int attack_delay;
	int wait_for_attack;

   public:
	EnnemiYoshiDCA();

	virtual void update();
	virtual void onAvance();
	virtual void onMeure();
	virtual void onTire();
	virtual void onCarbonise();

	virtual void estTouche(Tir* tir);
};
