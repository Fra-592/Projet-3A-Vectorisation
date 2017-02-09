#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "libs/io.h"
#include "libs/transformee.h"
#include "libs/squelettisation.h"
#include "libs/vectorisation.h"
#include "libs/simplification.h"


int imgHeight;
int imgWidth;


int main(int argc, char const *argv[])
{
	if(argc != 3 || (strcmp(argv[1], "help") == 0) || (atoi(argv[2]) < 0) || atoi(argv[2]) > 100)
	{
		printf("Vous avez oublié de préciser l'image à utiliser et/ou le seuil!\n\n"
			"Rappel: ce programme s'utilise de la manière suivante: ./vectorisation <nom de l'image> <seuil>\n"
			"\t-L'image appelée doit être placée dans le dossier img et être au format bitmap 24 bits."
			"Son nom doit être de moins de 64 caractères.\n"
			"\t-Le seuil est un entier entre 0 (pas de simplification) et 100 (simplification maximale).\n\n");
		list_images();
	}
	else
	{
		//Extraction des arguments
		char imgName[64];
		strcpy(imgName, argv[1]);
		int seuil = atoi(argv[2]);
		

		//Obtention de l'image et de ses infos
		chdir("img");
		if(!is_valid_img(imgName))
		{
			printf("Image invalide. Tapez ./vectorisation help pour obtenir de l'aide\n");
			return(-1);
		}
		imgWidth = get_image_width(imgName);
		imgHeight = get_image_height(imgName);


		//Création de la matrice qui va représenter l'image
		int **image;
		int i, j;

		image = malloc(imgHeight * sizeof(int *));

		for(i = 0; i < imgHeight; i++)
		{
			image[i] = malloc(imgWidth * sizeof(int));
		}
		get_image_pixels(imgName, image);
		chdir("..");


		//Création de la matrice qui va contenir la transformée en distance
		int **transformee;

		transformee = malloc(imgHeight * sizeof(int *));

		for(i = 0; i < imgHeight; i++)
		{
			transformee[i] = malloc(imgWidth * sizeof(int));
			for(j = 0; j < imgWidth; j++)
			{
				transformee[i][j] = image[i][j]==0?0:1024;
			}
		}
		transformee_distance(image, transformee);


		//Création du tableau de flags
		t_flag **flags;

		flags = malloc(imgHeight * sizeof(void *));

		for(i = 0; i < imgHeight; i++)
		{
			flags[i] = malloc(imgWidth * sizeof(t_flag));
			for(j = 0; j < imgWidth; j++)
			{
				flags[i][j] = 0;
			}
		}
		squelettisation(transformee, flags);

		// Libération de l'image
		for(i = 0; i < imgHeight; i++)
		{
			free(image[i]);
		}
		free(image);

		// Libération de la transformée en distance
		for(i = 0; i < imgHeight; i++)
		{
			free(transformee[i]);
		}
		free(transformee);


		tp_vects vecteurs;
		vecteurs = extraire_vecteurs(flags);

		// Libération du tableau de flags
		for(i = 0; i < imgHeight; i++)
		{
			free(flags[i]);
		}
		free(flags);

		// Simplification de Douglas-Peucker;
		simplification(vecteurs, seuil);

		// Export vers un fichier .tex
		print_tex(imgName, vecteurs);

		//Libération des vecteurs
		tp_vects vect = vecteurs;
		tp_vect point, point2;
		while(vect)
		{
			point = vect->vecteur;
			while(point)
			{
				point2 = point;
				point = point2->suiv;
				free(point2);
			}
			vecteurs = vect;
			vect = vecteurs->suiv;
			free(vecteurs);
		}
	}

	return(0);
}