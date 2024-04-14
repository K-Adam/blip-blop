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
	virtual void estTouche(Shot* tir);
	virtual void update();
	virtual void onCarbonise();
	virtual void affiche();
};

class EnemyBisouJediHero : public EnemyBisouJedi {
   public:
	bool intro;

	EnemyBisouJediHero() : intro(true) { pv = 6000; };

	virtual void update() {
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

	virtual void estTouche(Shot* tir) {
		EnemyBisouJedi::estTouche(tir);

		if (pv <= 0) {
			game_flag[FLAG_USER1] = 2;
			game_flag[FLAG_GEN_OFF] = 1;
		}
	}
};
