#pragma once

#include "entities/enemies/enemy.h"

#define tolerance 5

class EnemySnorkInBlackBoss : public Enemy {
   private:
	int nb_tir;
	int position;
	int type_attack;
	int dx;
	bool attack;

	int x_shark;
	int y_shark;
	bool shark;
	bool saut;

   public:
	EnemySnorkInBlackBoss();

	virtual void update();
	virtual void onAvance();
	virtual void onTombe();
	virtual void onMeure();
	virtual void onTire();
	virtual void affiche();

	virtual void estTouche(Shot* tir);
};
