#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "libs/io.h"

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

		int imgWidth = get_image_width(imgName);
		int imgHeight = get_image_height(imgName);


		//Création de la matrice qui va représenter l'image
		int **image;
		int i;
		image = malloc(imgHeight * sizeof(int *));
		for(i = 0; i < imgHeight; i++)
		{
			image[i] = malloc(imgWidth * sizeof(int));
		}

		get_image_pixels(imgName, image);

		//Libération de mémoire
		for(i = 0; i < imgHeight; i++)
		{
			free(image[i]);
		}
		free(image);
	}

	return(0);
}