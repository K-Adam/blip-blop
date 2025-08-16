#pragma once

#include "entities/enemies/enemy.h"

#define PRINCESSE_SPEED 1

class EnemyPrincess : public Enemy {
   private:
	bool avance;
	bool aide;

   public:
	EnemyPrincess();
	void update() override;
	virtual void onAvance();
	virtual void onMeure();

	void estTouche(Shot* tir) override;
};
