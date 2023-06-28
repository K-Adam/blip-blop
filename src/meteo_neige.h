#pragma once

#include "sprite.h"

class MeteoNeige : public Sprite
{
public:
	int		xbase;
	int		phi;
	int		xwide;

	void update();
};
