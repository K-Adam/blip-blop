#pragma once

#include "entities/enemies/enemy.h"

class EnemySmurfHefty : public Enemy {
   public:
	int etape_shoot;
	int wait_shoot;
	int dx;
	int dypunch;

	EnemySmurfHefty();

	virtual void update();
	virtual void onAvance();
	virtual void onTire();
	virtual void onTireHadoken();
	virtual void onTireDragonPunch();
	virtual void onTireHurricanKick();
	virtual void estTouche(Shot* tir);
	virtual void onCarbonise();
	virtual void onMeure();
};
