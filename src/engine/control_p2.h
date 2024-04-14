#pragma once

#include "engine/input.h"
#include "engine/controller.h"
#include "engine/control_alias.h"

class ControlP2 : public Controller {
   protected:
   public:
	virtual int gauche() const { return in.scanAlias(ALIAS_P2_LEFT); };

	virtual int haut() const { return in.scanAlias(ALIAS_P2_UP); };

	virtual int droite() const { return in.scanAlias(ALIAS_P2_RIGHT); };

	virtual int bas() const { return in.scanAlias(ALIAS_P2_DOWN); };

	virtual int fire() const { return in.scanAlias(ALIAS_P2_FIRE); };

	virtual int saut() const { return in.scanAlias(ALIAS_P2_JUMP); };

	virtual int super() const { return in.scanAlias(ALIAS_P2_SUPER); };
};
