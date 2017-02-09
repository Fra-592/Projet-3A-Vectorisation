#ifndef _TRANSFORMEE_H
	#define _TRANSFORMEE_H
	
/*
	Remplit la matrice "transformee" avec la transformée en distance de la matrice "image"
*/
	void transformee_distance(int **image, int **transformee);

/*
	Fonctions utilitaires
*/
	int min(int x, int y);
	int max(int x, int y);

	extern int imgHeight;
	extern int imgWidth;
#endif
