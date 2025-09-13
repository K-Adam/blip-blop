#pragma once

#include "entities/enemies/enemy.h"

class EnemyBisouJedi : public Enemy {
   public:
	int wait_shoot;
	int etape_shoot;
	bool a_epee;

	EnemyBisouJedi();
	virtual void onAvance();
	virtual void onMeure();
	void takeHit(Shot* shot) override;
	void update() override;
	virtual void onCarbonise();
	void render() override;
};

class EnemyBisouJediHero : public EnemyBisouJedi {
   public:
	bool intro;

	EnemyBisouJediHero() : intro(true) { hp = 6000; };

	void update() override {
		if (intro) {
			if (x > offset + 450) {
				EnemyBisouJedi::onAvance();
				col_on = false;
			} else {
				intro = false;
				game_flag[FLAG_USER1] = 1;
				game_flag[FLAG_GEN_OFF] = 0;
			}
		} else {
			EnemyBisouJedi::update();
		}
	};

	void takeHit(Shot* shot) override {
		EnemyBisouJedi::takeHit(shot);

		if (hp <= 0) {
			game_flag[FLAG_USER1] = 2;
			game_flag[FLAG_GEN_OFF] = 1;
		}
	}
};
