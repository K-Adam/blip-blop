#pragma once

#include "entities/enemies/enemy.h"

class EnemyPacman : public Enemy {
   private:
	int shot;
	int shot_step;
	int shot_tick;

	int dx;

   public:
	EnemyPacman();
	void update() override;
	int damage() override;
	void render() override;
	void takeHit(Shot* shot) override;

	bool count() override { return false; };
};
