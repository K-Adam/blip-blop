#pragma once

#include "entities/enemies/enemy.h"

class EnemyBisouZombie : public Enemy {
   public:
	bool my_phase;
	bool my_phase2;
	int etape2;

	EnemyBisouZombie();
	virtual void onCoupe();
	virtual void onAvance();
	virtual void onTombe();
	virtual void onRampe();
	virtual void onMeure();
	virtual void onCarbonise();
	virtual void onCarboniseHaut();
	void estTouche(Shot* tir) override;
	void update() override;
};
