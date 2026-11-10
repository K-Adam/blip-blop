#pragma once

#include "entities/enemies/enemy.h"

class EnemyBisouJedi : public Enemy {
   public:
	int wait_shoot;
	int shoot_step;
	bool has_sword;

	EnemyBisouJedi();
	virtual void onWalk();
	virtual void onDie();
	void takeHit(Shot* shot) override;
	void update() override;
	virtual void onCharred();
	void render() override;
};

class EnemyBisouJediHero : public EnemyBisouJedi {
   public:
	bool intro;

	EnemyBisouJediHero() : intro(true) { hp = 6000; };

	void update() override {
		if (intro) {
			if (x > level().camera.offset + 450) {
				EnemyBisouJedi::onWalk();
				col_on = false;
			} else {
				intro = false;
				level().rules.flags[FLAG_USER1] = 1;
				level().rules.flags[FLAG_GEN_OFF] = 0;
			}
		} else {
			EnemyBisouJedi::update();
		}
	};

	void takeHit(Shot* shot) override {
		EnemyBisouJedi::takeHit(shot);

		if (hp <= 0) {
			level().rules.flags[FLAG_USER1] = 2;
			level().rules.flags[FLAG_GEN_OFF] = 1;
		}
	}
};
