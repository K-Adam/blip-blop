/******************************************************************
 *
 *
 *		------------
 *		  Event.h
 *		------------
 *
 *		Classe Evenement
 *
 *		La classe mère de tous les évenements
 *
 *
 *		Prosper / LOADED -   2 Aout 2000
 *
 *
 *
 ******************************************************************/

#pragma once

#include "globals.h"

class Event {
   public:
	int x_activation;

	inline virtual bool aReveiller() { return (x_activation <= offset); };

	inline virtual bool aActiver() { return true; };

	virtual void doEvent() = 0;
};
