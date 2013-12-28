
#ifndef _HiScores_
#define _HiScores_

#ifndef NULL
#define NULL	0
#endif

const int HS_NB_SCORES		= 10;
const int HS_NAME_LENGTH	= 20;

class HiScores {
protected:

	char *	names[HS_NB_SCORES];
	int		scores[HS_NB_SCORES];

	/*
		Le premier appel � cette fonction crypte les donn�es. Le
		deuxi�me les d�cryptes, etc...

		ATTENTION : il ne faut surtout pas lire les donn�es crypt�es!
		(risque de cha�ne sans '\0')
	*/

public:
	void	crypte();

	HiScores() {
		for (int i = 0; i < HS_NB_SCORES; i++)
			names[i] = new char[HS_NAME_LENGTH];

		init();
	};

	~HiScores() {
		for (int i = 0; i < HS_NB_SCORES; i++)
			if (names[i] != NULL)
				delete [] names[i];
	};


	/*
		Retourne le nom du joueur de rang N (0 �tant le meilleur)
	*/
	char *	getName(int n) const {
		if (n >= 0 && n < HS_NB_SCORES)
			return names[n];
		else
			return NULL;
	};

	/*
		Retourne le score du joueur de rang N (0 �tant le meilleur)
	*/
	int		getScore(int n) const {
		if (n >= 0 && n < HS_NB_SCORES)
			return scores[n];
		else
			return 0;
	};

	/*
		Retourne vrai si le score pass� en param�tre est assez
		�lev� pour rentrer dans le HiScore
	*/
	bool	isGood(int scr) {
		return (scores[HS_NB_SCORES - 1] < scr);
	};


	/*
		Initialise la table des scores avec des scores bidons
		et des "LOADED" partout
	*/
	void	init();


	/*
		Ajoute le score (et le nom) pass� en
		param�tre � la table
	*/
	void	add(int scr, const char * name);


	/*
		Sauvegarde les scores dans un fichier crypt�!
	*/
	bool	save(const char * file);

	/*
		Charge les scores dans un fichier crypt�!
	*/
	bool	load(const char * file);
};

#endif