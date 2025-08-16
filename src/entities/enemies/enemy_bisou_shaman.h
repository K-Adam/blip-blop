#pragma once

#include "entities/enemies/enemy.h"

class EnemyBisouShaman : public Enemy {
   public:
	int wait_shoot;
	int etape_shoot;
	int nb_lueurs;
	int nb_touch;
	int n_pos;

	EnemyBisouShaman();
	virtual void onAvance();
	virtual void onLeveBaton();
	virtual void onBaisseBaton();
	virtual void onTire();
	virtual void onMeure();
	virtual void onCarbonise();
	void estTouche(Shot* tir) override;
	void update() override;
	void affiche() override;
	virtual void onTele();
};
