
#pragma once

#include "sprite.h"

class Bulle : public Sprite
{
public:
	int		xbase;
	int		phi;
	int		dphi;

	Bulle();
	virtual void update();
};
