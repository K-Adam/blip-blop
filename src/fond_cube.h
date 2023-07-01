/******************************************************************
 *
 *
 *		---------------------------
 *		    FondCube.h
 *		---------------------------
 *
 *		//les point d'interogation chez mario et Co
 *
 *		Mephisto / LOADED -   V 0.2 - 12 Janvier 2001
 *
 *
 *
 ******************************************************************/

#pragma once

class FondCube : public Sprite {
   private:
	static int etape_synchro;
	static bool synchro;
	static int ss_etape_synchro;

   public:
	virtual void update();
};
