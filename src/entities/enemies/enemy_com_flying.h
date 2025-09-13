#pragma once

#include "entities/enemies/enemy.h"

class EnemyCOMFlying : public Enemy {
   private:
	int speed;
	bool attack;
	int y_target;

   public:
	EnemyCOMFlying();
	void update() override;
	virtual void onWalk();
	virtual void onDie();
};
