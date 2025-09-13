#pragma once

#include "entities/shots/shot.h"

class ShotShamanColumn : public Shot {
   public:
	int duree_vie;
	int status;	 // 0 created, 1 normal, 2 destroyed

	ShotShamanColumn();

	int damage() const override { return 1; };

	int enflame() const override { return 0; };

	void setDir(int d) override { dir = d; };

	void update() override;
	virtual void deplace();
};
