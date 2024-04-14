#pragma once

#include "entities/shots/shot.h"

class TirBouleChaman : public Tir {
   public:
	int dx;
	bool ok;
	bool boom;

	TirBouleChaman();

	virtual int degats() const { return 1; };

	virtual int enflame() const { return 0; };

	virtual void setDir(int d) { dir = d; };

	virtual void update();
};
