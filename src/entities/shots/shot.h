#pragma once

#ifndef FACING
#define FACING_LEFT 0
#define FACING_RIGHT 1
#endif

#include "entities/sprite.h"

class Shot : public Sprite {
   public:
	virtual int damage() const { return 1; };

	virtual int enflame() const { return 0; };

	virtual void setDir(int d) { dir = d; };

	virtual void onKill(){};

	virtual int getSplatterDir() const { return dir / 2; };

	inline virtual void onHit() {
		destroyed = true;
		col_on = false;
	};

	inline virtual void onHit(int pts) {
		(void)pts;
		destroyed = true;
		col_on = false;
	};

	virtual ~Shot(){};
};
