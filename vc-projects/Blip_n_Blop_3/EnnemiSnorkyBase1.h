/******************************************************************
*
*
*		-----------------------
*		    EnnemiSnorkyBase1.h
*		-----------------------
*
*
*
*		Mephisto / LOADED -   V 0.1 - 12 Decembre 2000
*
*
*
******************************************************************/

#ifndef _EnnemiSnorkyBase1_
#define _EnnemiSnorkyBase1_

//-----------------------------------------------------------------------------
//		Headers
//-----------------------------------------------------------------------------

#include "ennemi.h"

//-----------------------------------------------------------------------------
//		Constantes
//-----------------------------------------------------------------------------

#define SNORKY_BASE_SPEED		2

//-----------------------------------------------------------------------------
//		D�finition de la classe EnnemiSnorkyBase1
//-----------------------------------------------------------------------------

class EnnemiSnorkyBase1 : public Ennemi {
public:

	int wait_for_shoot;
	int shoot_delay;

	EnnemiSnorkyBase1();

	virtual void update();
	virtual void onAvance();
	virtual void onTombe();
	virtual void onMeure();
	virtual void onTire();
	virtual void onCarbonise();

	virtual void estTouche(Tir * tir);
};

#endif