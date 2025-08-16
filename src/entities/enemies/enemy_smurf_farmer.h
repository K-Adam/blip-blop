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

	inline void affiche() override {
		Enemy::affiche();
		if (etat == ETAT_MEURE && tete_etape < 5) {
			if (dir == SENS_GAUCHE)
				draw(x_mort, y_mort, pbk_ennemis[169 + tete_etape]);
			else
				draw(x_mort, y_mort, pbk_ennemis[164 + tete_etape]);
		}
	}

	void update() override;
	virtual void onAvance();
	virtual void onMeure();
	virtual void onCarbonise();

	void estTouche(Shot* tir) override;
};
