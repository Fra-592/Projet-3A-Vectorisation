#ifndef _VECTORISATION_H
	#define _VECTORISATION_H
	#include "squelettisation.h"

	// Définition des vecteurs
	typedef struct s_vect
	{
		int x;
		int y;
		struct s_vect *suiv;
	} t_vect, *tp_vect;

	// Définition des listes de vecteurs
	typedef struct t_vects_liste
	{
		tp_vect vecteur;
		struct t_vects_liste *suiv;
	} t_vects, *tp_vects;


	// Extrait les différents vecteurs à partir du squelette
	tp_vects extraire_vecteurs(t_flag **flags);
#endif
