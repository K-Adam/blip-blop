#pragma once

#include "entities/enemies/enemy.h"

class EnemyBisouHeart : public Enemy {
   public:
	EnemyBisouHeart();
	virtual void onAvance();
	virtual void onMeure();
	virtual void onPart();
	void estTouche(Shot* tir) override;
	void update() override;
};
