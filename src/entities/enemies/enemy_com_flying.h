#pragma once

#include "entities/enemies/enemy.h"

class EnemyCOMFlying : public Enemy {
   private:
	int speed;
	bool attack;
	int y_cible;

   public:
	EnemyCOMFlying();
	virtual void update();
	virtual void onAvance();
	virtual void onMeure();
};
