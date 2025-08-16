#pragma once

#include "entities/enemies/enemy.h"

#define YOSHI_SPEED 3

class EnemyYoshi : public Enemy {
   private:
	bool attack;  // False: shell attack, true: tongue contact
	int attack_delay;
	int wait_for_attack;

   public:
	EnemyYoshi();

	void update() override;
	virtual void onAvance();
	virtual void onTombe();
	virtual void onMeure();
	virtual void onAttaque();
	virtual void onTire();
	virtual void onCarbonise();

	void estTouche(Shot* tir) override;
};
