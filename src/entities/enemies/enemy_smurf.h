#pragma once

#include "entities/enemies/enemy.h"

class EnemySmurf : public Enemy {
   public:
	int wait_for_jump;
	int jump_delay;
	int dy_saut;
	int speed;

	EnemySmurf();

	void update() override;
	virtual void onAvance();
	virtual void onSaute();
	virtual void onMeure();
	virtual void onCarbonise();

	void takeHit(Shot* shot) override;
};
