#pragma once

#include "entities/enemies/enemy.h"

class EnemyBisouHeart : public Enemy {
   public:
	EnemyBisouHeart();
	virtual void onAvance();
	virtual void onMeure();
	virtual void onPart();
	void takeHit(Shot* shot) override;
	void update() override;
};
