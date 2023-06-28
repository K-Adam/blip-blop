/******************************************************************
*
*
*		-----------------------
*		    EnnemiLemmings.h
*		-----------------------
*
*
*
*		Prosper / LOADED -   V 0.1 - 3 Aout 2000
*
*
*
******************************************************************/

#pragma once

//-----------------------------------------------------------------------------
//		Headers
//-----------------------------------------------------------------------------

#include "ennemi_lemmings.h"

//-----------------------------------------------------------------------------
//		Définition de la classe
//-----------------------------------------------------------------------------

class EnnemiLemmingsVolant : public EnnemiLemmings
{
public:

	EnnemiLemmingsVolant()
	{
		y = -5;
	};
};