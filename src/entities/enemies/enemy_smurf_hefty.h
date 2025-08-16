#pragma once

#include "entities/enemies/enemy.h"

class EnemySmurfHefty : public Enemy {
   public:
	int etape_shoot;
	int wait_shoot;
	int dx;
	int dypunch;

	EnemySmurfHefty();

	void update() override;
	virtual void onAvance();
	virtual void onTire();
	virtual void onTireHadoken();
	virtual void onTireDragonPunch();
	virtual void onTireHurricanKick();
	void estTouche(Shot* tir) override;
	virtual void onCarbonise();
	virtual void onMeure();
};
