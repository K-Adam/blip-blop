#pragma once

#include "entities/enemies/enemy.h"

#define LATENCE_BOUGE 1

class EnemyWeight : public Enemy {
   private:
	int nb_colision;

   public:
	EnemyWeight();
	void update() override;
	int damage() override;
	virtual void onMeure();
	void takeHit(Shot* shot) override;
	bool count() override { return false; };
};
