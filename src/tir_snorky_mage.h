#pragma once

#include "tir.h"

class TirSnorkyMage : public Tir {
   public:
	int dx;

	TirSnorkyMage();

	virtual int degats() const { return 1; };

	virtual int enflame() const { return 0; };

	virtual void setDir(int d) { dir = d; };

	virtual void update();
};
