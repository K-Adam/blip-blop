#pragma once

#include "entities/enemies/enemy.h"

class EnemyBisouCloud : public Enemy {
   public:
	int wait_shoot;
	int shoot_step;

	EnemyBisouCloud();
	virtual void onNormal();
	void update() override;
	virtual void onDieWait();
	virtual void onDieFall();
	virtual void onDieEnd();
};
