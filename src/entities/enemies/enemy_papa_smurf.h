#pragma once

#include "entities/enemies/enemy.h"

class EnemyPapaSmurf : public Enemy {
   public:
	Sprite* target;
	int wait_change_target;
	int dx;
	int attack_wanted;

	EnemyPapaSmurf();
	virtual void updateTarget();
	void update() override;
	virtual void updateChoice();
	virtual void onWalk();
	virtual void onAtata();
};
