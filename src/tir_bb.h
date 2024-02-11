#pragma once

#include "joueur.h"
#include "tir.h"

#include "bbconst.h"

class TirBB : public Tir {
   public:
	Joueur* joueur;
	int dxReferentiel;

	TirBB() : dxReferentiel(0){};
	virtual int degats() const = 0;
	virtual int enflame() const = 0;

	virtual void aTue() { joueur->addKilled(); };

	inline virtual void aTouche(int pts) {
		a_detruire = true;
		col_on = false;
		joueur->addPoints(pts);
	};

	virtual void setDir(int d);
};
