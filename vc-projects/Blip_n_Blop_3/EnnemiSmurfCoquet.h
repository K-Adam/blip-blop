/******************************************************************
*
*
*		-----------------------
*		EnnemiSmurfCoquet.h
*		-----------------------
*		Schtroumph de base.
*		Sp�cificit� : Saute (beaucoup !) plus haut.
*					  Vitesse tir�e al�atoirement � la cr�ation.
*
*		Mortimus / LOADED -   V 0.1 - 26 Octobre 2000
*
*
*
******************************************************************/

#ifndef _EnnemiSmurfCoquet_
#define _EnnemiSmurfCoquet_

//-----------------------------------------------------------------------------
//		Headers
//-----------------------------------------------------------------------------

#include "EnnemiSmurf.h"

//-----------------------------------------------------------------------------
//		D�finition de la classe
//-----------------------------------------------------------------------------

class EnnemiSmurfCoquet : public EnnemiSmurf
{
public:

	EnnemiSmurfCoquet()
	{
		dy_saut = -6 ;
		speed = 1 + rand() % 2 ;
	}

	inline virtual void affiche()
	{
		EnnemiSmurf::affiche() ;
		if (etat != ETAT_MEURE && etat != ETAT_CARBONISE) {
			switch (dir) {
				case SENS_GAUCHE :
					draw(x + 3, y - 31, pbk_ennemis[151]) ;
					break ;
				case SENS_DROITE :
					draw(x - 3, y - 31, pbk_ennemis[151]) ;
					break ;
			}
		}
	}
};

#endif