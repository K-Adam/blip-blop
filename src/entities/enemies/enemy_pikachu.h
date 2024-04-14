#pragma once

#include "entities/enemies/enemy.h"

class EnnemiPikachu : public Ennemi {
   public:
	EnnemiPikachu();
	virtual void update();
	virtual void onAvance();
	virtual void onMeure();
	virtual void onCarbonise();
	virtual void estTouche(Tir* tir);
};
