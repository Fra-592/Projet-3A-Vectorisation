#include "simplification.h"
#include <stdlib.h>

float distance(int a, int b, int x, int y)
{
	float result;
	result = (float)fabs(a * x - y + b)/sqrt(a*a + 1);
	return(result);
}

void douglas_peucker(tp_vect vecteur, int fin, int seuil)
{
	tp_vect vect_save = vecteur, vect_temp;
	int x1, y1, x2, y2, i_max, i;
	float a, b, max_dist, dist;

	// Détermination de l'équation de la droite sous la forme y = a*x + b <=> a*x - y + b = 0
	x1 = vecteur->x;
	y1 = vecteur->y;
	while(vecteur->suiv)
	{
		vecteur = vecteur->suiv;
	}
	x2 = vecteur->x;
	y2 = vecteur->y;
	a = (float)(y2 - y1)/(x2 - x1);
	b = (y1 - a*x1);

	// Détermination du point le plus éloigné et de sa distance à la droite
	vecteur = vect_save->suiv;
	i_max = 1;
	max_dist = 0;
	for(i = 1; i < fin; i++)
	{
		dist = distance(a, b, vecteur->x, vecteur->y);
		if(dist > max_dist)
		{
			max_dist = dist;
			i_max = i;
		}
		vecteur = vecteur->suiv;
	}

	// Si on peut simplifier directement
	if(max_dist < ((float)seuil/50))
	{
		vecteur = vect_save->suiv;
		while(vecteur->suiv)
		{
			vect_temp = vecteur;
			vecteur = vecteur->suiv;
			free(vect_temp);
		}
		vect_save->suiv = vecteur;
	}
	else
	{
		vecteur = vect_save;
		for(i = 0; i < i_max; i++)
		{
			vecteur = vecteur->suiv;
		}
		if((fin - i_max)> 1)
		{
			douglas_peucker(vecteur,fin - i_max, seuil);
		}

		if(i_max > 1)
		{
			douglas_peucker(vect_save, i_max, seuil);
		}
		
	}
	return;
}

void simplification(tp_vects liste, int seuil)
{
	tp_vect vect;
	while(liste)
	{
		douglas_peucker(liste->vecteur, taille(liste->vecteur), seuil);
		liste = liste->suiv;
	}
}