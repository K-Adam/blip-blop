#pragma once

#include "entities/enemies/enemy.h"

class EnemySmurfette : public Enemy {
   public:
	int etape_shoot;
	int wait_shoot;

	EnemySmurfette();

	virtual void update();
	virtual void onAvance();
	virtual void onTire();
	virtual void estTouche(Shot* tir);
	virtual void onCarbonise();
	virtual void onMeure();
};
