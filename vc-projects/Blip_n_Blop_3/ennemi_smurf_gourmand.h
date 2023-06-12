/******************************************************************
*
*
*		---------------------------
*		    EnnemiSmurfGourmand.h
*		---------------------------
*
*		Le schtroumpf de base!
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

#include "enemy.h"


//-----------------------------------------------------------------------------
//		Définition de la classe EnnemiSmurf
//-----------------------------------------------------------------------------

class EnnemiSmurfGourmand : public Ennemi
{
public:
	int		wait_for_shoot;
	int		shoot_delay;
	int		xcible;
	int		ycible;

	EnnemiSmurfGourmand();

	virtual void update();
	virtual void onNormal();
	virtual void onTire();
	virtual void onMeure();
	virtual void onHold();
	virtual void onCarbonise();

	virtual void estTouche(Tir * tir);
};
