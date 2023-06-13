/******************************************************************
*
*
*		--------------------
*		  EventGenEnnemi.h
*		--------------------
*
*		Classe Evenement Ennemi
*
*		La classe evenement qui crée un ennemi
*
*
*		Prosper / LOADED -   5 Aout 2000
*
*
*
******************************************************************/

#pragma once

#include "event.h"

class EventGenEnnemi : public Event
{
public:
	int		x;
	int		y;
	int		id_ennemi;
	int		sens;
	int		capacite;
	int		periode;
	bool	tmp;


	virtual void doEvent();
};
