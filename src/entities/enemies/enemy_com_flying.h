#pragma once

#include "entities/enemies/enemy.h"

class EnemyCOMFlying : public Enemy {
   private:
	int speed;
	bool attack;
	int y_cible;

   public:
	EnemyCOMFlying();
	void update() override;
	virtual void onAvance();
	virtual void onMeure();
};
