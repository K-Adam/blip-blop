#pragma once

#include "enemy.h"

class EnnemiCOMVolant : public Ennemi {
   private:
	int speed;
	bool attack;
	int y_cible;

   public:
	EnnemiCOMVolant();
	virtual void update();
	virtual void onAvance();
	virtual void onMeure();
};
