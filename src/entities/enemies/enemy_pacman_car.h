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
	void update() override;
	int degats() override;
	virtual void tombeVoiture();
	void affiche() override;
	void estTouche(Shot* tir) override;
};
