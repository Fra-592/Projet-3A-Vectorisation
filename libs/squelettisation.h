#ifndef _SQUELETTISATION_H
	#define _SQUELETTISATION_H
	// Définition des flags
	typedef enum e_flag
	{
		FOND = 1, CONTOUR = 1<<1, INTERNE = 1<<2, MULTIPLE = 1<<3, SQUELETTE = 1<<4
	} t_flag;

	// Fonction de squelettisation de la transformée en distance
	void squelettisation(int **transformee, t_flag **flags);
#endif
