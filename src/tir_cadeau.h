#pragma once

#include "tir.h"

class TirCadeau : public Tir {
   public:
	TirCadeau();
	virtual int degats() const { return 1; };

	virtual int enflame() const { return 0; };

	virtual void setDir(int d) { dir = d; };

	virtual void update();
};
