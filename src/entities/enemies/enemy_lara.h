#pragma once

#include "entities/enemies/enemy.h"

#define LARA_TIR_SPEED 10
#define ATTACK_DELAY 10

class EnemyLara : public Enemy {
   private:
	int numero_image_buste;
	int attack_delay;
	int wait_for_attack;
	int attack_step;
	int attack_ss_etape;
	int attack_type;
	int attack_phase;
	int nb_pluie;
	int x_cible;
	int y_target;
	int pics;
	int tete_rayman;

   public:
	EnemyLara();
	void update() override;
	virtual void onAvance();
	virtual void onMeure();
	virtual void onTire();

	void render() override;
	void takeHit(Shot* shot) override;
};
