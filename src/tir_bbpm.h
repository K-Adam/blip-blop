#pragma once

#include "joueur.h"
#include "tir_bb.h"

class TirBBPM : public TirBB {
   public:
	inline virtual int degats() const { return 75; };

	inline virtual int enflame() const { return 0; };

	virtual void update();
};
