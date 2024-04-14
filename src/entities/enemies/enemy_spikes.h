#pragma once

#include "entities/enemies/enemy.h"

class EnemySpikes : public Enemy {
   private:
	int y_origine;

   public:
	EnemySpikes();
	virtual void update();
	virtual void affiche();
	virtual void colFromPic();
	virtual void estTouche(Shot* tir);
	virtual int degats() { return 5; };

	virtual bool count() { return false; };
};
