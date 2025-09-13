#pragma once

#include "entities/enemies/enemy.h"

#define SONIC_SPEED 7
#define SONIC_CHARGE_SPEED 8

class EnemySonic : public Enemy {
   private:
	int speed;
	int etape_speed;  // Acceleration delay
	int attack_delay;
	int wait_for_attack;
	int attack_phase;
	int attack_type;
	int x_cible;
	int y_target;
	int ss_etape_attack;
	int etape_attack;
	int nb_tir;
	int vie_sous_critique;
	int vie_critique;
	int vie_sur_critique;
	int ring;
	int wait_sang_tombe;

   public:
	EnemySonic();
	void update() override;
	virtual void onAvance();
	virtual void onMeure();
	virtual void onSaute();
	virtual void onCharge();
	virtual void onAttackhorizontal();
	virtual void onAttackvertical();
	virtual void onAttackepine();
	virtual void onAttackeclair();
	virtual void onBalancering();
	virtual void balance_epines();
	virtual void onCarbonise();
	void takeHit(Shot* shot) override;
};
