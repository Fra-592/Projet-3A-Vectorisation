#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "libs/io.h"
#include "libs/transformee.h"
#include "libs/squelettisation.h"


int imgHeight;
int imgWidth;


int main(int argc, char const *argv[])
{
	if(argc != 3 || (strcmp(argv[1], "help") == 0) || (atoi(argv[2]) < 0) || atoi(argv[2]) > 100)
	{
		printf("Vous avez oublié de préciser l'image à utiliser et/ou le seuil!\n\n"
			"Rappel: ce programme s'utilise de la manière suivante: ./vectorisation <nom de l'image> <seuil>\n"
			"\t-L'image appelée doit être placée dans le dossier img et être au format bitmap."
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
		int i;

		image = malloc(imgHeight * sizeof(int *));
		for(i = 0; i < imgHeight; i++)
		{
			image[i] = malloc(imgWidth * sizeof(int));
		}

		get_image_pixels(imgName, image);


		//Création de la matrice qui va contenir la transformée en distance
		int **transformee;

		transformee = malloc(imgHeight * sizeof(int *));
		for(i = 0; i < imgHeight; i++)
		{
			transformee[i] = malloc(imgWidth * sizeof(int));
		}

		transformee_distance(image, transformee);


		//Création du tableau de flags
		t_flag **flags;
		int j;

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
		printf("\n");
		for(i = 0; i < imgHeight; i++)
		{
			for(j = 0; j < imgWidth; j++)
			{
				printf("%d ",flags[i][j]);
			}
			printf("\n");
		}



		//Libération de mémoire
		for(i = 0; i < imgHeight; i++)
		{
			free(image[i]);
		}
		free(image);

		for(i = 0; i < imgHeight; i++)
		{
			free(transformee[i]);
		}
		free(transformee);

		for(i = 0; i < imgHeight; i++)
		{
			free(flags[i]);
		}
		free(flags);
	}

	return(0);
}