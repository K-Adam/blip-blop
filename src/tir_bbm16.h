#pragma once

#include "joueur.h"
#include "tir_bb.h"

class TirBBM16 : public TirBB {
   public:
	inline virtual int degats() const { return 40; };

	inline virtual int enflame() const { return 0; };

	virtual void update();
};
