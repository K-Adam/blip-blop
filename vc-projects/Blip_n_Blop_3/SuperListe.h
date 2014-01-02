/******************************************************************
*
*
*		----------------
*		   SuperListe.h
*		----------------
*
*
*		Liste de pointeurs (void*) doublement cha�n�e
*		MAIS � sens unique
*
*
*		Prosper / LOADED -   V 1.0 - 28 Juin 2000
*
*
*
******************************************************************/


#ifndef _SuperListe_
#define _SuperListe_

//-----------------------------------------------------------------------------
//		Structure CELLULE pour rendre la liste plus jolie
//-----------------------------------------------------------------------------

struct CELLULE {
	CELLULE *	prev;
	void *		data;
	CELLULE *	next;
};

//-----------------------------------------------------------------------------
//		La classe SuperListe en personne
//-----------------------------------------------------------------------------

class SuperListe
{
protected:
	CELLULE *	tete;		// Tete de la liste
	CELLULE *	obs;		// Observateur
	int			nb_elem;	// Nombre d'�lements
public:
	static CELLULE *trash;
	static int		nb_lists;

public:

	// Constructeur normal
	//
	SuperListe();

	// Destructeur
	~SuperListe();


	// Met l'observateur sur la t�te
	//
	inline void 	start()
	{
		obs = tete;
	};


	// Avance l'observateur
	//
	inline void 	suivant()
	{
		if (obs != NULL) obs = obs->next;
	};


	// Rajoute une cellule en t�te de liste
	//
	void	ajoute(void * nouvo);


	// Supprime la cellule observ�e
	//
	// ! ATTENTION !
	//
	// La suppression avance l'observateur !!!
	//
	void	supprime();


	// Supprime la cellule observ�e, mais pas la valeur contenue
	//
	void *	supprimePorc();


	// Vide la liste
	//
	void	vide();
	void	vide_porc();


	// Renvoit le nombre d'�l�ments contenus
	//
	inline int	taille() const
	{
		return nb_elem;
	};


	// Indique si la fin de la liste a �t� atteinte
	//
	inline bool	fin() const
	{
		return (obs == NULL);
	};


	// Indique VRAI si la liste est vide (ne contient aucune cellule)
	//
	inline bool	estVide() const
	{
		return (tete == NULL);
	};


	// Renvoit la valeur de la cellule observ�e
	//
	inline void *	info() const
	{
		if (obs != NULL)
			return (obs->data);
		else
			return NULL;
	};


	// Trie la liste selon la fonction donn�e en param�tre
	//
	void	(trier(int (*fonc)(const void *, const void*)));
};

#endif
