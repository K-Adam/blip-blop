#pragma once

#include "entities/enemies/enemy.h"

#define LATENCE_BOUGE 1

class EnemyWeight : public Enemy {
   private:
	int nb_colision;

   public:
	EnemyWeight();
	void update() override;
	int degats() override;
	virtual void onMeure();
	void estTouche(Shot* tir) override;
	bool count() override { return false; };
};
