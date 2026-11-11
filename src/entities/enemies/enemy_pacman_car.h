#pragma once

#include "entities/enemies/enemy.h"

#define CAR_GRAVITY_DELAY 5
#define GRAVITY_MAX_CAR 8

class EnemyPacmanCar : public Enemy {
   private:
	int wait_end;

   public:
	EnemyPacmanCar();
	void update() override;
	int damage() override;
	virtual void fallCar();
	void render() override;
	void takeHit(Shot* shot) override;
};
