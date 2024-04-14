#pragma once

#include "entities/enemies/enemy.h"

class EnemyRaymanHead : public Enemy {
   private:
	int dx;

   public:
	// methodes "habituelles"
	EnemyRaymanHead();
	virtual void update();
	virtual void onAvance();
	virtual void onMeure();

	virtual bool count() { return false; };

	virtual int degats();

	virtual void estTouche(Shot* tir);
};
