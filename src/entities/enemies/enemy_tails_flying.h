#pragma once

#include "entities/enemies/enemy.h"

#define TAILS_SPEED 3
#define TAILS_FLY_SPEED 6

class EnemyTailsFlying : public Enemy {
   private:
	int speed;		  // vitesse courante
	int etape_speed;  // delai d'acceleration
	int fly_delay;
	int wait_for_fly;
	int x_cible;
	int y_cible;
	bool attack;  // attaque ou se met a la bonne hauteur
	bool fly;
	int direction;
	int placer;

   public:
	EnemyTailsFlying();
	virtual void update();
	virtual void onAvance();
	virtual void onMeure();
	virtual void onTombe();
	virtual void onVolle();
	virtual void onCarbonise();
	virtual void estTouche(Shot* tir);
};
