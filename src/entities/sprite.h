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
	const Picture* pic;
	bool destroyed;
	int dir;
	int grav_tick;

	int wait_bubble;

	int x1;	 // Collision box
	int y1;
	int x2;
	int y2;
	bool col_on;

	Sprite();

	inline virtual int getX() const { return x; };
	inline virtual int getY() const { return y; };
	inline virtual int getAnimStep() const { return anim_step; };
	inline virtual const Picture* getPic() const { return pic; };
	inline virtual bool isDestroyed() const { return destroyed; };

	inline virtual bool isOnBloodyWall() const	// Blood stained wall
	{
		return bloody_wall(x, y);
	}

	inline virtual int animate(const int* tab, int nb_steps, int delay) {
		anim_tick += 1;
		anim_tick %= delay;

		if (anim_tick == 0) anim_step += 1;

		if (anim_step < 0) anim_step = 0;

		anim_step %= nb_steps;
		return tab[anim_step];
	};

	inline virtual void render() { draw(x, y, pic); };

	inline virtual void updateDestroyed() {
		if (x < offset - 250 || x > offset + 900 || y < -200 || y > 600) destroyed = true;
	};

	virtual void update() = 0;

	virtual void fall();
	virtual void fall2();
	virtual void walk(int speed);
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
