#pragma once

#include "entities/enemies/enemy.h"

#define LUIGI_SPEED 6
#define LATENCE_LUIGI_GRAVITE 12
#define EXPLOSE_DELAY 180
#define NOMBRE_GICLURES 90

#define MULTIPLICATEUR_RECUL_SOL 20
#define MULTIPLACATEUR_VITESSE_AU_SOL 100
#define MULTIPLICATEUR_RECUL_AERIEN 15
#define MULTIPLICATEUR_VITESSE_AERIEN 80

class EnemyLuigi : public Enemy {
   private:
	int speed;
	int etape_speed;  // Acceleration delay

	int attack_delay;
	int wait_for_attack;
	int attack_step;
	int attack_type;
	int ss_etape_attack;
	int etape_attack;
	int nb_tir;
	int encaissement;

   public:
	EnemyLuigi();
	void update() override;
	virtual void onAvance();
	virtual void onMeure();
	virtual void onSaute();
	virtual void onAttack();
	virtual void onCarbonise();
	virtual void tombe_luigi();
	void takeHit(Shot* shot) override;
};
