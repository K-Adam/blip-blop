#pragma once

#ifndef SENS
#define SENS_GAUCHE 0
#define SENS_DROITE 1
#endif

#include "entities/sprite.h"

class Tir : public Sprite {
   public:
	virtual int degats() const { return 1; };

	virtual int enflame() const { return 0; };

	virtual void setDir(int d) { dir = d; };

	virtual void aTue(){};

	virtual int getGiclureDir() const { return dir / 2; };

	inline virtual void aTouche() {
		a_detruire = true;
		col_on = false;
	};

	inline virtual void aTouche(int pts) {
		(void)pts;
		a_detruire = true;
		col_on = false;
	};

	virtual ~Tir(){};
};
