#pragma once

#include "entities/enemies/enemy.h"

#define DIABOLO_KICK_SPEED 8
#define DIABOLO_GRAVITY_DELAY 5
#define DIABOLO_GRAVITY_MAX 8

class EnemyDiabolo : public Enemy {
   public:
	int wait_for_attack;
	int attack_delay;
	int dx;
	int attack_type;

	EnemyDiabolo();

	void update() override;
	virtual void onWalk();
	virtual void onFall();
	virtual void onDie();
	virtual void onAttack();
	virtual void onKick();
	virtual void onTornado();
	virtual void onKneeStrike();
	virtual void onCharred();
	virtual void fall_diabolo();
	void colFromPic() override;

	void takeHit(Shot* shot) override;
};
