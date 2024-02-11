#pragma once

#include "tir.h"

class TirColonneChaman : public Tir {
   public:
	int duree_vie;
	int etat;  // 0:cree - 1:normal - 2:detruit

	TirColonneChaman();

	virtual int degats() const { return 1; };

	virtual int enflame() const { return 0; };

	virtual void setDir(int d) { dir = d; };

	virtual void update();
	virtual void deplace();
};
