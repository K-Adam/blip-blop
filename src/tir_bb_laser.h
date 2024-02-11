#pragma once

#include "joueur.h"
#include "tir_bb.h"

class TirBBLaser : public TirBB {
   protected:
	int largeur;
	int hauteur;
	int base;

   public:
	TirBBLaser();

	virtual int getGiclureDir() const { return dir; };

	inline virtual int degats() const { return 20; };

	inline virtual int enflame() const { return 0; };

	inline virtual void setEtape(int e) { etape = e; };

	inline virtual void aTouche(int pts) { joueur->addPoints(pts); };

	inline virtual void aTouche(){};

	virtual void affiche();
	virtual void setDir(int d);
	virtual void update();
	virtual bool collision(const Sprite* s) const;
};
