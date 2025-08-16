#pragma once

#include "entities/enemies/enemy.h"

class EnemyPikachu : public Enemy {
   public:
	EnemyPikachu();
	void update() override;
	virtual void onAvance();
	virtual void onMeure();
	virtual void onCarbonise();
	void estTouche(Shot* tir) override;
};
