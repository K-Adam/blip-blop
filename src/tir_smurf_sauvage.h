#pragma once

#include "tir.h"

class TirSmurfSauvage : public Tir {
   public:
	int dx;
	int dy;

	TirSmurfSauvage();
	virtual int degats() const { return 1; };

	virtual int enflame() const { return 0; };

	virtual void setDir(int d) { dir = d; };

	virtual void update();
};
