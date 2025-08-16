#pragma once

#include "entities/enemies/enemy.h"

#define KNUKLES_SPEED 3
#define KNUKLES_CHARGE_SPEED 6
#define KNUKLES_JUMP_SPEED 12
#define KNUKLES_GRAVITE_MAX 7

class EnemyKnuckles : public Enemy {
   private:
	int speed;
	int etape_speed;  // Acceleration delay
	int charge_delay;
	int wait_for_charge;

   public:
	EnemyKnuckles();
	void update() override;
	virtual void onAvance();
	virtual void onMeure();
	virtual void onTombe();
	virtual void onCarbonise();
	virtual void onCharge();
	virtual void onSaute();
	void estTouche(Shot* tir) override;
	void tombe() override;
};
