#pragma once

#include "entities/enemies/enemy.h"
#include "entities/shots/shot_rayman_fist.h"

#define RAYMAN_SPEED 3
#define RAYMAN_CRITICAL_SPEED 6
#define RAYMAN_POING1_SPEED 7
#define RAYMAN_POING2_SPEED 10
#define PV_SOUS_CRITIQUE 10000
#define PV_CRITIQUE 5000
#define PV_SUR_CRITIQUE 1000

class EnemyRayman : public Enemy {
   private:
	int attack_delay;
	int wait_for_attack;
	int attack_etape;
	int x_cible;
	int range_poing;
	int poing1;
	int poing2;
	int pieds;
	int dorkemon;

	ShotRaymanFist* tir_poing;

   public:
	EnemyRayman();
	virtual void update();
	virtual void onAvance();
	virtual void onMeure();
	virtual void onTombe();
	virtual void onTire();
	virtual void estTouche(Shot* tir);
};
