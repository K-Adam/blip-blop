#pragma once

#include "entities/enemies/enemy.h"

#define LEM_SPEED 1

class EnemyLemmings : public Enemy {
   public:
	bool suicide;

	EnemyLemmings();

	void update() override;
	virtual void onAvance();
	virtual void onTombe();
	virtual void onMeure();
	virtual void onCarbonise();

	void estTouche(Shot* tir) override;
};
