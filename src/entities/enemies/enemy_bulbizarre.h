#pragma once

#include "entities/enemies/enemy.h"

class EnemyBulbizarre : public Enemy {
   public:
	EnemyBulbizarre();
	virtual void update();
	virtual void onAvance();
	virtual void onMeure();
	virtual void onCarbonise();
	virtual void estTouche(Shot* tir);
};
