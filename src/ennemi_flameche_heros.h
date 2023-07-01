/******************************************************************
 *
 *
 *		------------------------
 *		  EnnemiFlamecheHeros.h
 *		------------------------
 *
 *
 *
 *		Mephisto / LOADED -   V 0.1 - 14 Janvier 2001
 *
 *
 *
 ******************************************************************/

#pragma once

#include "enemy.h"

class EnnemiFlamecheHeros : public Ennemi {
   private:
	int dx;
	int dorkeball;

   public:
	EnnemiFlamecheHeros();
	virtual void update();
	virtual void onAvance();
	virtual void onMeure();
	virtual void estTouche(Tir* tir);
};
