/******************************************************************
*
* 
*		----------------
*		   TirGateau.h
*		----------------
*			
*
*
*		Prosper / LOADED -   V 0.1 - 17 Juillet 2000
*
*
*
******************************************************************/

#ifndef _TirGateau_
#define _TirGateau_

//-----------------------------------------------------------------------------
//		Headers
//-----------------------------------------------------------------------------

#include "tir.h"

//-----------------------------------------------------------------------------
//		D�finition de la classe Tir
//-----------------------------------------------------------------------------

class TirGateau : public Tir
{
public:
	int		dx;

	TirGateau();
	
	virtual int degats() const
	{
		return 1;
	};

	virtual int enflame() const
	{
		return 0;
	};

	virtual void setDir( int d)
	{
		dir = d;
	};

	virtual void update();

};

#endif