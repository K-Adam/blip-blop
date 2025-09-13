#pragma once

#include "entities/enemies/enemy.h"

class EnemyPikachu : public Enemy {
   public:
	EnemyPikachu();
	void update() override;
	virtual void onWalk();
	virtual void onDie();
	virtual void onCharred();
	void takeHit(Shot* shot) override;
};
