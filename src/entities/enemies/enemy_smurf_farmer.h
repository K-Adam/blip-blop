#pragma once

#define SMURF_PAYSAN_SPEED 4

#include "entities/enemies/enemy.h"

class EnemySmurfFarmer : public Enemy {
   public:
	int tete_ss_etape;
	int tete_etape;
	int x_mort;
	int y_mort;

	EnemySmurfFarmer();

	inline void render() override {
		Enemy::render();
		if (status == ETAT_MEURE && tete_etape < 5) {
			if (dir == FACING_LEFT)
				draw(x_mort, y_mort, pbk_enemies[169 + tete_etape]);
			else
				draw(x_mort, y_mort, pbk_enemies[164 + tete_etape]);
		}
	}

	void update() override;
	virtual void onAvance();
	virtual void onMeure();
	virtual void onCarbonise();

	void takeHit(Shot* shot) override;
};
