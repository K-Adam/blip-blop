#pragma once

#include "enemy.h"

class EnnemiSmurfCigogne : public Ennemi
{
public:

	int		base_y;
	int		base_x;
	bool	first;
	int		teta;

	EnnemiSmurfCigogne();

	virtual void update();
};
