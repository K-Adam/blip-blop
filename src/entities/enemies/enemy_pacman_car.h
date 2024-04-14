#pragma once

#include "entities/enemies/enemy.h"

#define LATENCE_GRAVITE_VOITURE 5
#define GRAVITE_MAX_VOITURE 8

class EnemyPacmanCar : public Enemy {
   private:
	int invoc_pacman;
	int wait_end;

   public:
	EnemyPacmanCar();
	virtual void update();
	virtual int degats();
	virtual void tombeVoiture();
	virtual void affiche();
	virtual void estTouche(Shot* tir);
};
