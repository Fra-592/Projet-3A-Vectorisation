#ifndef _simplification_H
	#define _simplification_H
	#include "vectorisation.h" // Pour les types tp_vect et tp_vects
	#include <math.h>

/*
	Simplifie les vecteurs de la liste passée en paramètre via l'algorithme de Douglas-Peucker
*/
	void simplification(tp_vects liste, int seuil);
#endif