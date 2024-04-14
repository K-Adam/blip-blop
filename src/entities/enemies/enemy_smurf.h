#pragma once

#include "entities/enemies/enemy.h"

class EnemySmurf : public Enemy {
   public:
	int wait_for_jump;
	int jump_delay;
	int dy_saut;
	int speed;

	EnemySmurf();

	virtual void update();
	virtual void onAvance();
	virtual void onSaute();
	virtual void onMeure();
	virtual void onCarbonise();

	virtual void estTouche(Shot* tir);
};
