#pragma once

#include "ennemi_smurf.h"

class EnnemiSmurfParesseux : public EnnemiSmurf {
   public:
	EnnemiSmurfParesseux() { dy_saut = -4; }

	inline virtual void affiche() { EnnemiSmurf::affiche(); }
};
