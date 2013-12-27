/******************************************************************
*
*
*		----------------
*		   BenMaths.h
*		----------------
*
*
*		Fonctions math�matiques pr�calcul�e
*		Gain approximatif : 2000 � 2500%
*
*		Cf. BenMaths.cpp pour plus d'infos
*
*		Prosper / LOADED -   V 0.1
*
*
*
******************************************************************/


#ifndef _BenMaths_
#define _BenMaths_

//-----------------------------------------------------------------------------
//		Constantes
//-----------------------------------------------------------------------------

#ifndef PI
#define	PI			3.141592654	// C'est PI, utilis� pour la conversion
#endif								// depgr�s<->radians
#ifndef NULL
#define	NULL		0			// Well...
#endif

#define		COSINUS		10			// Pour l'utilisation de bCos[] et bSin[]
#define		PREC_COS	1			// Demande le pr�calcul des cosinus
#define		PREC_SIN	2			// Demande le pr�calcul des sinus
#define		PREC_ROOT	4			// Demande le pr�calcul des racines carr�es
#define		PREC_ALL	0xFFFFFFFF	// Demande le pr�calcul de toutes les fonctions

//-----------------------------------------------------------------------------
//		D�claration externe des tableaux
//-----------------------------------------------------------------------------

#ifndef BENMATHS_CPP_FILE
extern int *	bCos;
extern int *	bSin;
extern char *	bSqr;
#endif

//-----------------------------------------------------------------------------
//		Prototypes
//-----------------------------------------------------------------------------

void preCalcMathsFunctions();
void preCalcMathsFunctions(int f);
void preCalcMathsFunctions(int f, int nb);
void freeMathsFunctions();

#endif
