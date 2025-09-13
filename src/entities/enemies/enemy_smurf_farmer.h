#pragma once

#define SMURF_FARMER_SPEED 4

#include "entities/enemies/enemy.h"

class EnemySmurfFarmer : public Enemy {
   public:
	int head_tick;
	int head_step;
	int x_death;
	int y_death;

	EnemySmurfFarmer();

	inline void render() override {
		Enemy::render();
		if (status == STATE_DYING && head_step < 5) {
			if (dir == FACING_LEFT)
				draw(x_death, y_death, pbk_enemies[169 + head_step]);
			else
				draw(x_death, y_death, pbk_enemies[164 + head_step]);
		}
	}

	void update() override;
	virtual void onWalk();
	virtual void onDie();
	virtual void onCharred();

	void takeHit(Shot* shot) override;
};
