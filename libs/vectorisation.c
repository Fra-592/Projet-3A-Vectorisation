#include <malloc.h>
#include "vectorisation.h"

// ADT liste de vecteurs
tp_vects creer_liste()
{
	return(NULL);
}

tp_vects ajouter_vecteur(tp_vects liste, tp_vect vecteur)
{
	tp_vects temp;
	
	temp = malloc(sizeof(t_vects));
	
	temp->vecteur = vecteur;
	temp->suiv = liste;

	return(temp);
}



// ADT vecteurs
tp_vect creer_vecteur()
{
	return(NULL);
};

tp_vect ajouter_point(tp_vect vecteur, int x, int y)
{
	tp_vect temp;

	temp = malloc(sizeof(t_vect));

	temp->x = x;
	temp->y = y;
	temp->suiv = vecteur;

	return(temp);
}

int longueur(tp_vect vecteur)
{
	int cpt;

	cpt = 0;
	while(vecteur->suiv)
	{
		cpt++;
		vecteur = vecteur->suiv;
	}
	return(cpt);
}

int nb_voisins(t_flag **flags, int *x, int *y)
{
	int x2 = *x, y2 = *y, i, j;
	int cpt = 0;
	for(i = max(x2-1, 0); i < min(x2+2, imgHeight); i++)
	{
		for(j = max(y2-1, 0); j < min(y2+2, imgWidth); j++)
		{
			if(((i!=x2) || (j!=y2)) 
				&& (flags[i][j] & SQUELETTE))
			{
				cpt++;
				*x = i;
				*y = j;
			}
		}
	}
	return(cpt);
}

tp_vect prochain_point(t_flag **flags, tp_vect vecteur, int x, int y)
{
	int i = x, j = y;
	int voisins = nb_voisins(flags, &i, &j);
	if(voisins == 0)
	{
		flags[x][y] = flags[x][y] ^ SQUELETTE;
		vecteur = ajouter_point(vecteur, x, y);
	}
	else if(voisins == 1)
	{
		flags[x][y] = flags[x][y] ^ SQUELETTE;
		vecteur = ajouter_point(vecteur, x, y);
		vecteur = prochain_point(flags, vecteur, i, j);
	}
	else
	{
		flags[x][y] = flags[x][y] ^ SQUELETTE;
		vecteur = ajouter_point(vecteur, x, y);
		vecteur = prochain_point(flags, vecteur, i, j);
		flags[x][y] = flags[x][y] ^ SQUELETTE;
	}
	return(vecteur);
}

tp_vects extraire_vecteurs(t_flag **flags)
{	
		tp_vects liste;
		tp_vect vecteur;
		int i, j;

		liste = creer_liste();
		for(i = 0; i < imgHeight; i++)
		{
			for(j = 0; j < imgWidth; j++)
			{
				if(flags[i][j] & SQUELETTE)
				{
					vecteur = creer_vecteur();
					vecteur = prochain_point(flags, vecteur, i, j);
					
					if(longueur(vecteur) == 0)
					{
						free(vecteur);
					}
					else
					{
						liste = ajouter_vecteur(liste, vecteur);
					}
				}
			}
		}
	return(liste);
}