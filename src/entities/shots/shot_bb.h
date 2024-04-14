#pragma once

#include "entities/player.h"
#include "entities/shots/shot.h"

#include "core/constants.h"

class ShotBB : public Shot {
   public:
	Player* joueur;
	int dxReferentiel;

	ShotBB() : dxReferentiel(0){};
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
