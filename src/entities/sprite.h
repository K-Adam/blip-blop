#pragma once

#include "engine/picture.h"
#include "level/world.h"
#include "core/assets.h"

class Sprite {
   public:
	int x;
	int y;
	int dy;
	int anim_step;
	int anim_tick;
	Picture* pic;
	bool destroyed;
	int dir;
	int lat_grav;

	int wait_bubble;

	int x1;	 // Collision box
	int y1;
	int x2;
	int y2;
	bool col_on;

	Sprite();

	inline virtual int getX() const { return x; };
	inline virtual int getY() const { return y; };
	inline virtual int getEtape() const { return anim_step; };
	inline virtual Picture* getPic() const { return pic; };
	inline virtual bool isDestroyed() const { return destroyed; };

	inline virtual bool estSurMur() const  // Blood stained wall
	{
		return bloody_wall(x, y);
	}

	inline virtual int animate(const int* tab, int nb_etapes, int latence) {
		anim_tick += 1;
		anim_tick %= latence;

		if (anim_tick == 0) anim_step += 1;

		if (anim_step < 0) anim_step = 0;

		anim_step %= nb_etapes;
		return tab[anim_step];
	};

	inline virtual void render() { draw(x, y, pic); };

	inline virtual void updateDestroyed() {
		if (x < offset - 250 || x > offset + 900 || y < -200 || y > 600) destroyed = true;
	};

	virtual void update() = 0;

	virtual void fall();
	virtual void tombe2();
	virtual void marche(int speed);
	virtual void colFromPic();
	virtual void noCol();

	inline virtual bool collision(const Sprite* s) const {
		if (!col_on || !s->col_on)
			return false;
		else
			return !((s->x2 < x1) || (s->x1 > x2) || (s->y2 < y1) || (s->y1 > y2));
	};

	virtual ~Sprite(){};
};
