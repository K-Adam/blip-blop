#pragma once

#include "entities/enemies/enemy.h"

#define MARIO_SPEED 5
#define MARIO_CHARGE_SPEED 7
#define LATENCE_MARIO_GRAVITE 12
#define MARIO_PV_SOUS_CRITIQUE 17500
#define MARIO_PV_CRITIQUE 11500
#define MARIO_PV_SUR_CRITIQUE 8500

class EnemyMario : public Enemy {
   private:
	int speed;
	int etape_speed;  // Acceleration delay
	int fireball;	  // Fireball held in hands
	int hologramme;

	int attack_delay;
	int wait_for_attack;
	int attack_step;
	int attack_ss_etape;
	int attack_type;
	int attack_phase;
	int nb_pluie;
	int x_cible;
	int y_target;

   public:
	EnemyMario();
	void update() override;
	virtual void onAvance();
	virtual void onMeure();
	virtual void onSaute();
	virtual void onCharge();
	virtual void onCarbonise();
	virtual void tombe_mario();

	virtual void onTirehorizontale();
	virtual void onTireverticale();
	virtual void onTirecross();

	virtual void onRafalehorizontale();
	virtual void onRafalehorizontaledouble();
	virtual void onRafalehorizontaleconstante();
	virtual void onRafalehorizontalechercheuse();

	virtual void onRafaleverticale();
	virtual void onRafaleverticaleinverser();
	virtual void onRafaleverticaledouble();
	virtual void onRafaleverticalechercheuse();
	virtual void onRafaleverticaleconstante();
	virtual void onPluiedefeu();

	virtual void onRafaleberserker();
	virtual void onRafalecrosschercheuse();
	virtual void onRafalecross();

	// Fireballs only travel horizontally
	virtual inline void boule_de_feu(int vitesse);

	void render() override;
	void takeHit(Shot* shot) override;
};
