/******************************************************************
 *
 *
 *		---------------
 *		  GenBonus.h
 *		---------------
 *
 *
 *		Prosper / LOADED -   5 Aout 2000
 *
 *
 *
 ******************************************************************/

#pragma once

class GenBonus {
   public:
	int x;
	int periode;
	int t;
	int type;
	bool a_detruire;

	GenBonus();

	virtual void update();

	inline virtual bool aDetruire() { return a_detruire; };
};
