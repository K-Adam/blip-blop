#pragma once

#include "entities/enemies/enemy.h"

#define MARIO_SPEED 5
#define MARIO_CHARGE_SPEED 7
#define LATENCE_MARIO_GRAVITE 12
#define NABO_TRANSFORMATION 300
#define MARIO_ENDURANCE 80
#define MULTIPLICATEUR_RECUL_SOL 20
#define MULTIPLACATEUR_VITESSE_AU_SOL 100
#define MULTIPLICATEUR_RECUL_AERIEN 15
#define MULTIPLICATEUR_VITESSE_AERIEN 80

class EnemyMarioSwatter : public Enemy {
   private:
	int aide_luigi;
	int speed;
	int etape_speed;  // Acceleration delay

	int attack_delay;
	int wait_for_attack;
	int attack_step;
	int nabo_delay;

	int nabo;
	int encaissement;

   public:
	EnemyMarioSwatter();
	void update() override;
	virtual void onAvance();
	virtual void onMeure();
	virtual void onSaute();
	virtual void onCharge();
	virtual void onCarbonise();
	virtual void tombe_mario();
	void takeHit(Shot* shot) override;
};
