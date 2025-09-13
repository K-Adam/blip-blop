#pragma once

#include "entities/enemies/enemy.h"

class EnemyBisouShaman : public Enemy {
   public:
	int wait_shoot;
	int shoot_step;
	int nb_glows;
	int nb_touch;
	int n_pos;

	EnemyBisouShaman();
	virtual void onWalk();
	virtual void onRaiseStaff();
	virtual void onLowerStaff();
	virtual void onShoot();
	virtual void onDie();
	virtual void onCharred();
	void takeHit(Shot* shot) override;
	void update() override;
	void render() override;
	virtual void onTeleport();
};
