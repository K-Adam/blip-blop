#pragma once

#include "entities/enemies/enemy.h"

class EnemyPikachu : public Enemy {
   public:
	EnemyPikachu();
	virtual void update();
	virtual void onAvance();
	virtual void onMeure();
	virtual void onCarbonise();
	virtual void estTouche(Shot* tir);
};
