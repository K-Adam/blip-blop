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
	virtual void update();
	virtual int degats();
	virtual void affiche();
	virtual void estTouche(Shot* tir);

	virtual bool count() { return false; };
};
