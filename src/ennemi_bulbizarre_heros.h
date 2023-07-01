/******************************************************************
 *
 *
 *		---------------------------
 *		  EnnemiBulbizarreHeros.h
 *		---------------------------
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

class EnnemiBulbizarreHeros : public Ennemi {
   private:
	int dx;
	int dorkeball;

   public:
	EnnemiBulbizarreHeros();
	virtual void update();
	virtual void onAvance();
	virtual void onMeure();
	virtual void estTouche(Tir* tir);
};
