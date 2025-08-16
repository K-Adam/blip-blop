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
	void estTouche(Shot* tir) override;
	void update() override;
	virtual void onCarbonise();
	void affiche() override;
};

class EnemyBisouJediHero : public EnemyBisouJedi {
   public:
	bool intro;

	EnemyBisouJediHero() : intro(true) { pv = 6000; };

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

	void estTouche(Shot* tir) override {
		EnemyBisouJedi::estTouche(tir);

		if (pv <= 0) {
			game_flag[FLAG_USER1] = 2;
			game_flag[FLAG_GEN_OFF] = 1;
		}
	}
};
