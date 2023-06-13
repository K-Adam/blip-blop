/******************************************************************
*
*
*		--------------------
*		  EventGenBonus.h
*		--------------------
*
*
*
*		Prosper / LOADED -   5 Aout 2000
*
*
*
******************************************************************/

#pragma once

#include "event.h"
#include "gen_bonus.h"

class EventGenBonus : public Event
{
public:
	int		type;
	int		periode;


	virtual void doEvent()
	{
		GenBonus * gb = new GenBonus();

		gb->periode = periode;
		gb->type = type;
		gb->x = x_activation;

		list_gen_bonus.emplace_back(gb);
	};
};
