#pragma once

#include "entities/shots/shot.h"

class TirSoniceclair : public Tir {
   public:
	TirSoniceclair();

	virtual int degats() const { return 1; };

	virtual int enflame() const { return 0; };

	virtual void setDir(int d) { dir = d; };

	virtual void update();
	virtual void affiche();
};
