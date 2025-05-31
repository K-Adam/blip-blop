#pragma once

#include "entities/enemies/enemy.h"

#define MARIO_SPEED 5
#define LATENCE_MARIO_GRAVITE 12
#define NB_CLIGNO 4

class EnemyMarioHologram : public Enemy {
   private:
	int speed;
	int etape_speed;  // Acceleration delay
	int fireball;	  // Fireball held in hands
	int wait_for_cligno;
	int cligno_delay;
	int decalage_cligno;

	int attack_delay;
	int wait_for_attack;
	int attack_etape;
	int attack_type;
	int attack_phase;
	int x_cible;
	int y_cible;

   public:
	EnemyMarioHologram();
	virtual void update();
	virtual void onAvance();
	virtual void onMeure();
	virtual void onSaute();
	virtual void onCharge();
	virtual void onCarbonise();
	virtual void onTire();
	virtual void onTireverticale();
	virtual void tombe_mario();
	virtual inline void boule_de_feu();
	virtual void affiche();
	virtual void estTouche(Shot* tir);
};
