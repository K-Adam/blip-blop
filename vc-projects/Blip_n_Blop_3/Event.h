/******************************************************************
*
*
*		------------
*		  Event.h
*		------------
*
*		Classe Evenement
*
*		La classe m�re de tous les �venements
*
*
*		Prosper / LOADED -   2 Aout 2000
*
*
*
******************************************************************/

#ifndef _Event_
#define _Event_

#include "Globals.h"

class Event
{
public:
	int		x_activation;

	inline virtual bool aReveiller()
	{
		return (x_activation <= offset);
	};

	inline virtual bool aActiver()
	{
		return true;
	};

	virtual void doEvent() = 0;
};

#endif
