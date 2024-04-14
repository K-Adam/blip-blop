#pragma once

#include "engine/input.h"
#include "engine/controller.h"
#include "engine/control_alias.h"

class ControlP1 : public Controller {
   protected:
   public:
	virtual int gauche() const { return in.scanAlias(ALIAS_P1_LEFT); };

	virtual int haut() const { return in.scanAlias(ALIAS_P1_UP); };

	virtual int droite() const { return in.scanAlias(ALIAS_P1_RIGHT); };

	virtual int bas() const { return in.scanAlias(ALIAS_P1_DOWN); };

	virtual int fire() const { return in.scanAlias(ALIAS_P1_FIRE); };

	virtual int saut() const { return in.scanAlias(ALIAS_P1_JUMP); };

	virtual int super() const { return in.scanAlias(ALIAS_P1_SUPER); };
};
