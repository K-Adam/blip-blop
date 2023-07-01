/******************************************************************
 *
 *
 *		----------------
 *		    TirBB.h
 *		----------------
 *
 *		Pour les tirs de Blip/Blop
 *
 *
 *		Prosper / LOADED -   V 0.1 - 17 Juillet 2000
 *
 *
 *
 ******************************************************************/

#pragma once

//-----------------------------------------------------------------------------
//		Headers
//-----------------------------------------------------------------------------

#include "joueur.h"
#include "tir.h"

//-----------------------------------------------------------------------------
//		Constantes
//-----------------------------------------------------------------------------

#include "bbconst.h"

//-----------------------------------------------------------------------------
//		Définition de la classe Tir
//-----------------------------------------------------------------------------

class TirBB : public Tir {
   public:
	Joueur* joueur;
	int dxReferentiel;

	TirBB() : dxReferentiel(0){};
	virtual int degats() const = 0;
	virtual int enflame() const = 0;

	virtual void aTue() { joueur->addKilled(); };

	inline virtual void aTouche(int pts) {
		a_detruire = true;
		col_on = false;
		joueur->addPoints(pts);
	};

	virtual void setDir(int d);
};
