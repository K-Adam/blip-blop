#pragma once

#include "entities/enemies/enemy.h"

class EnemyPacman : public Enemy {
   private:
	int shot;
	int etape_tir;
	int ss_etape_tir;

	int dx;

   public:
	EnemyPacman();
	void update() override;
	int damage() override;
	void render() override;
	void takeHit(Shot* shot) override;

	bool count() override { return false; };
};
