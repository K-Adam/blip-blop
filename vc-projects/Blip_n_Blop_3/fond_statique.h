#pragma once

#include "sprite.h"

class FondStatique : public Sprite
{
public:

	virtual void update()
	{
		if (x < 320)
			a_detruire = true;
	};
};
