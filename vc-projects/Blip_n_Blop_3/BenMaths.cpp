/******************************************************************
*
* 
*		----------------
*		   BenMaths.h
*		----------------
*			
*	
*		Fonctions math�matiques pr�calcul�e
*		(pour aller plus vite!)
*
*		
*		Pour l'instant, il y a 3 fonctions :
*			- bCos	(1440 octets)
*			- bSin  (1440 octets)
*			- bSqr	(64 Ko)
*
*		Utilisation:
*
*			- appeler preCalcMathsFunctions() qui
*			  pr�calcule le tout.
*
*			- pour obtenir  Z=X*cos(Y) faire Z=(X*bCos[Y])>>COSINUS;
*			  0 <= Y < 360
*			  -2097152 < X < 2097151
*
*			- pour obtenir la racine carr�e de X, faire
*			  bSqr[X]
*			  0 < X < ROOT_MAX (defaut=0xFFFF) <= 0xFFFF
*
*
*			- appeler freeMathsFunctions() pour lib�rer
*			  la m�moire occup�e
*
*
*		Prosper / LOADED -   V 0.1
*
*
*
******************************************************************/

//-----------------------------------------------------------------------------
//		Protection pour �viter de d�clarer les tableaux 2 fois
//-----------------------------------------------------------------------------

#define BENMATHS_CPP_FILE

//-----------------------------------------------------------------------------
//		Headers
//-----------------------------------------------------------------------------

#include <math.h>
#include <iostream>
#include "benmaths.h"

//-----------------------------------------------------------------------------
//		Tableaux dynamiques globaux pour les fonctions
//-----------------------------------------------------------------------------

int *	bCos = NULL;
int *	bSin = NULL;
char *	bSqr = NULL;

//-----------------------------------------------------------------------------
// Nom: preCalcMathsFunctions()
// Desc: Pr�calcule les fonctions math�matiques par d�faut
//-----------------------------------------------------------------------------

void preCalcMathsFunctions()
{
	preCalcMathsFunctions(PREC_ALL, 0xFFFF);
}

//-----------------------------------------------------------------------------
// Nom: preCalcMathsFunctions()
// Desc: Pr�calcule les fonctions math�matiques choisie
//-----------------------------------------------------------------------------

void preCalcMathsFunctions(int f)
{
	preCalcMathsFunctions(f, 0xFFFF);
}

//-----------------------------------------------------------------------------
// Nom: preCalcMathsFunctions()
// Desc: Pr�calcule les fonctions math�matiques pr�cis�es avec 'n' racines
//-----------------------------------------------------------------------------

void preCalcMathsFunctions(int flags, int n)
{

	// Pr�calcul des cosinus
	if ( bCos == NULL && (flags & PREC_COS))
	{
		bCos = new int[360];
		
		for (int i=0; i<360; i++)
			bCos[i] = int(1024*cos(i/(180/PI)));
	}
	
	// Pr�calcul des sinus
	if ( bSin == NULL && (flags & PREC_SIN))
	{
		bSin = new int[360];
		for (int i=0; i<360; i++)
			bSin[i] = int(1024*sin(i/(180/PI)));

	}

	// Pr�calcul des racines carr�es
	if ( bSqr == NULL && (flags & PREC_ROOT))
	{
		int		nbRoot;

		if ( n > 0xFFFF)
			nbRoot = 0xFFFF;
		else
			nbRoot = n;

		bSqr = new char[nbRoot];
		
		for (int i=0; i<nbRoot; i++)
			bSqr[i] = char(sqrt(i));

	}


}

//-----------------------------------------------------------------------------
// Nom: freeMathsFunctions()
// Desc: Lib�re la m�moire occup�e par la pr�calculation
//-----------------------------------------------------------------------------

void freeMathsFunctions()
{
	if ( bCos != NULL)
	{
		delete [] bCos;
		bCos = NULL;
	}

	if ( bSin != NULL)
	{
		delete [] bSin;
		bSin = NULL;
	}

	if ( bSqr != NULL)
	{
		delete [] bSqr;
		bSqr = NULL;
	}
}
