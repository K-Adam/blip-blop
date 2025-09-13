#pragma once

#include "entities/enemies/enemy.h"

#define TAILS_SPEED 3
#define TAILS_FLY_SPEED 6

class EnemyTailsFlying : public Enemy {
   private:
	int speed;
	int etape_speed;  // Acceleration delay
	int fly_delay;
	int wait_for_fly;
	int x_cible;
	int y_target;
	bool attack;  // Attacking, otherwise moving to the right height
	bool fly;
	int direction;
	int placed;

   public:
	EnemyTailsFlying();
	void update() override;
	virtual void onAvance();
	virtual void onMeure();
	virtual void onTombe();
	virtual void onVolle();
	virtual void onCarbonise();
	void takeHit(Shot* shot) override;
};
