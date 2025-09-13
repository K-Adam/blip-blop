#pragma once

#include "entities/enemies/enemy.h"

#define DIABOLO_KICK_SPEED 8
#define LATENCE_DIABOLO_GRAVITE 5
#define DIABOLO_GRAVITE_MAX 8

class EnemyDiabolo : public Enemy {
   public:
	int wait_for_attack;
	int attack_delay;
	int dx;
	int attack_type;

	EnemyDiabolo();

	void update() override;
	virtual void onAvance();
	virtual void onTombe();
	virtual void onMeure();
	virtual void onAttack();
	virtual void onCoupdelatte();
	virtual void onTornade();
	virtual void onCoupdegenou();
	virtual void onCarbonise();
	virtual void tombe_diabolo();
	void colFromPic() override;

	void takeHit(Shot* shot) override;
};
