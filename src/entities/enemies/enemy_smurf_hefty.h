#pragma once

#include "entities/enemies/enemy.h"

class EnemySmurfHefty : public Enemy {
   public:
	int shoot_step;
	int wait_shoot;
	int dx;
	int dypunch;

	EnemySmurfHefty();

	void update() override;
	virtual void onWalk();
	virtual void onShoot();
	virtual void onShootHadoken();
	virtual void onShootDragonPunch();
	virtual void onShootHurricaneKick();
	void takeHit(Shot* shot) override;
	virtual void onCharred();
	virtual void onDie();
};
