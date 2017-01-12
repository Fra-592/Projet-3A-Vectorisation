#ifndef _IO_H
	#define _IO_H

	#define DEBUG
	#define BLANC 0
	#define NOIR 1

	// Renvoie une matrice d'entiers représentants l'image, 1 = noir, 0 = blanc
	int get_image_pixels(const char *chemin, int **image);
#endif
