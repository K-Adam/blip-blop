#pragma once

#include "entities/enemies/enemy.h"

class EnemyPacman : public Enemy {
   private:
	int tir;
	int etape_tir;
	int ss_etape_tir;

	int dx;

   public:
	EnemyPacman();
	void update() override;
	int degats() override;
	void affiche() override;
	void estTouche(Shot* tir) override;

	bool count() override { return false; };
};
