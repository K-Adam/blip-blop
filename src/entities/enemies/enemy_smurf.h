#pragma once

#include "entities/enemies/enemy.h"

class EnemySmurf : public Enemy {
   public:
	int wait_for_jump;
	int jump_delay;
	int dy_jump;
	int speed;

	EnemySmurf();

	void update() override;
	virtual void onWalk();
	virtual void onJump();
	virtual void onDie();
	virtual void onCharred();

	void takeHit(Shot* shot) override;
};
