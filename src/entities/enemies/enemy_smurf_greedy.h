#pragma once

#include "entities/enemies/enemy.h"

class EnemySmurfGreedy : public Enemy {
   public:
	int wait_for_shoot;
	int shoot_delay;
	int xcible;
	int ycible;

	EnemySmurfGreedy();

	void update() override;
	virtual void onNormal();
	virtual void onTire();
	virtual void onMeure();
	virtual void onHold();
	virtual void onCarbonise();

	void estTouche(Shot* tir) override;
};
