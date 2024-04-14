#pragma once

#include "entities/enemies/enemy.h"

#define LARA_TIR_SPEED 10
#define ATTACK_DELAY 10

class EnemyLara : public Enemy {
   private:
	int numero_image_buste;
	int attack_delay;
	int wait_for_attack;
	int attack_etape;
	int attack_ss_etape;
	int attack_type;
	int attack_phase;
	int nb_pluie;
	int x_cible;
	int y_cible;
	int pics;
	int tete_rayman;

   public:
	// methodes "habituelles"
	EnemyLara();
	virtual void update();
	virtual void onAvance();
	virtual void onMeure();
	virtual void onTire();

	virtual void affiche();
	virtual void estTouche(Shot* tir);
};
