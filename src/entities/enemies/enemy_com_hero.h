#pragma once

#include "entities/enemies/enemy.h"

class EnemyCOMHero : public Enemy {
   private:
	int dx;
	int dorkeball;

   public:
	EnemyCOMHero();
	void update() override;
	virtual void onAvance();
	virtual void onMeure();
	void takeHit(Shot* shot) override;
};
