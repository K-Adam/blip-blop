/******************************************************************
*
*
*		----------------
*		  LGXpacker.h
*		----------------
*
*		Classe LGXpacker
*
*		La classe LGXpacker permet de charger
*		et de lire des images LGX.
*
*		! ATTENTION !
*		Le LGX ne fonctionne qu'en mode 16 bits!!!
*
*
*
*		Prosper / LOADED -   V 0.1 - 23 Juin 2000
*
*
*
******************************************************************/

#ifndef _LGX_
#define _LGX_

//-----------------------------------------------------------------------------
//		Headers
//-----------------------------------------------------------------------------

#include <ddraw.h>
#include "DD_gfx.h"


//-----------------------------------------------------------------------------
//		Constantes pour la compression/décompression LGX - Version 0
//-----------------------------------------------------------------------------

#define LGX_RMAX_0	31		// Valeur maximale de la composante ROUGE
#define	LGX_GMAX_0	63		// Valeur maximale de la composante VERTE
#define	LGX_BMAX_0	31		// Valeur maximale de la composante BLEUE

#define	LGX_RDECAL_0	11	// Décalage vers la gauche du masque ROUGE (en LGX)
#define	LGX_GDECAL_0	5	// Décalage vers la gauche du masque VERT (en LGX)
#define	LGX_BDECAL_0	0	// Décalage vers la gauche du masque BLEU (en LGX)


//-----------------------------------------------------------------------------
//		Constantes pour la compression/décompression LGX - Version 1
//-----------------------------------------------------------------------------

#define LGX_RMAX_1	31		// Valeur maximale de la composante ROUGE
#define	LGX_GMAX_1	31		// Valeur maximale de la composante VERTE
#define	LGX_BMAX_1	31		// Valeur maximale de la composante BLEUE

#define	LGX_RDECAL_1	10	// Décalage vers la gauche du masque ROUGE (en LGX)
#define	LGX_GDECAL_1	5	// Décalage vers la gauche du masque VERT (en LGX)
#define	LGX_BDECAL_1	0	// Décalage vers la gauche du masque BLEU (en LGX)


//-----------------------------------------------------------------------------
//		Structure LGX_HEADER - correspond au header du fichier
//-----------------------------------------------------------------------------


struct LGX_HEADER {
	char			id[3];		// 'LGX'
	char			version;	// Numéro de version
	unsigned short	xsize;		// Largeur
	unsigned short	ysize;		// Hauteur
	unsigned short	depth;		// Profondeur en bits (16 pour l'instant)
};

//-----------------------------------------------------------------------------
//		Définition de la classe LGXpacker
//-----------------------------------------------------------------------------

class LGXpacker
{
private:
	unsigned short *	tab_0;
	unsigned short *	tab_1;
	unsigned short *	half_tone;

	int		rDecal;	// Décalages des masques par rapport au bit de droite
	int		gDecal;	// (avec la carte vidéo)
	int		bDecal;

	int		rMax;	// Maximum des composantes (avec la carte vidéo)
	int		gMax;
	int		bMax;

	int		rMask;	// Masques RGB
	int		gMask;
	int		bMask;


public:
	LGXpacker();
	~LGXpacker();

	bool init(IDirectDrawSurface7 * surf);

	bool createLGX_0(HDC hdc, const char * fic, int xs, int ys);
	bool createLGX_1(HDC hdc, const char * fic, int xs, int ys);

	int createLGX_0(HDC hdc, int xs, int ys, void * & ptr);
	int createLGX_1(HDC hdc, int xs, int ys, void * & ptr);

	IDirectDrawSurface7 * loadLGX(void * ptr, int flag = DDSURF_BEST, int * version = NULL);
	IDirectDrawSurface7 * loadLGX(const char * fic, int flag = DDSURF_BEST);

	int	findColor(COLORREF rgb);

	void halfTone(IDirectDrawSurface7 * surf, RECT * r);

	void closePaker();
};

#ifndef LGXPACKER_CPP_FILE
extern LGXpacker	LGXpaker;
#endif

#endif
