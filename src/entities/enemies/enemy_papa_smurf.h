#pragma once

#include "entities/enemies/enemy.h"

class EnemyPapaSmurf : public Enemy {
   public:
	Sprite* cible;
	int wait_change_cible;
	int dx;
	int attak_wanted;

	EnemyPapaSmurf();
	virtual void updateCible();
	void update() override;
	virtual void updateChoice();
	virtual void onAvance();
	virtual void onAtata();
};
