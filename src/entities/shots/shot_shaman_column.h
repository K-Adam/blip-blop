#pragma once

#include "entities/shots/shot.h"

class ShotShamanColumn : public Shot {
   public:
	int duree_vie;
	int etat;  // 0 created, 1 normal, 2 destroyed

	ShotShamanColumn();

	virtual int degats() const { return 1; };

	virtual int enflame() const { return 0; };

	virtual void setDir(int d) { dir = d; };

	virtual void update();
	virtual void deplace();
};
