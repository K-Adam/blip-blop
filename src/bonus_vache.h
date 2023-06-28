/******************************************************************
*
*
*		---------------
*		  BonusVache.h
*		---------------
*
*
*
*		Prosper / LOADED -   V 0.1 - 17 Juillet 2000
*
*
*
******************************************************************/

#pragma once

#include "bonus.h"

class BonusVache : public Bonus
{
public:

	BonusVache()
	{
		pic = pbk_misc[48];
		col_on = true;
	};

	virtual void estPris(Couille * c)
	{
		sbk_misc.play(1);

		c->nb_cow_bomb += 1;

		Bonus::estPris(c);
	};
};
