#pragma once

#include "sprite.h"

class MorceauSnorkyBaseTeteEntiere : public Sprite {
   public:
	MorceauSnorkyBaseTeteEntiere();

	virtual void update();
};

class MorceauSnorkyBaseTetePartieArriere : public Sprite {
   public:
	int dx;

	MorceauSnorkyBaseTetePartieArriere(int vx, int vy);

	virtual void update();
};

class MorceauSnorkyHyporidercorp : public Sprite {
   public:
	int dx;

	MorceauSnorkyHyporidercorp(int vx);

	virtual void update();
};

class MorceauSnorkyBaseTrompe : public Sprite {
   public:
	int dx;

	MorceauSnorkyBaseTrompe(int vx, int vy);

	virtual void update();
};
