#ifndef _VECTORISATION_H
	#define _VECTORISATION_H
	#include "squelettisation.h"
	#include "transformee.h" //Pour avoir min et max
/*
	Définition des vecteurs
*/
	typedef struct s_vect
	{
		int x;
		int y;
		struct s_vect *suiv;
	} t_vect, *tp_vect;

/*
	Définition des listes de vecteurs
*/
	typedef struct t_vects_liste
	{
		tp_vect vecteur;
		struct t_vects_liste *suiv;
	} t_vects, *tp_vects;

/*
	Fonctions liées au type tp_vect
*/
	tp_vect ajouter_point(tp_vect vecteur, int x, int y);
	tp_vect creer_vecteur();

/*
	Extrait les différents vecteurs à partir de la matrice de flags
*/
	tp_vects extraire_vecteurs(t_flag **flags);


	extern int imgHeight;
	extern int imgWidth;
#endif
