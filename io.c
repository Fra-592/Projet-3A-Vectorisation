#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "BMP/bmp_worker.h"
#include "io.h"

int **get_image_pixels(const char *chemin, int **image)
{
	//Ouverture du fichier
	FILE *fichier = fopen(chemin, "rb+");
	assert(fichier != NULL);
	
	#ifdef DEBUG
	if(fichier == NULL)
	{
			puts("Impossible d'ouvrir ce fichier.");
	}
	puts("Fichier ouvert avec succès.");
	#endif


	//Lecture des headers
    BmpWorker_fileHeader *fileHeader = (BmpWorker_fileHeader *) malloc(sizeof(BmpWorker_fileHeader));
	
	#ifdef DEBUG
	if(BmpWorker_FileHeader_read(fichier, fileHeader) != SUCCESS)
	{
		puts("Impossible d'ouvrir le header du fichier.");
	}
  	puts("Header du fichier ouvert lu avec succès.");
	#endif

	BmpWorker_infoHeader * imgHeader = (BmpWorker_infoHeader *) malloc(sizeof(BmpWorker_infoHeader));
	
	#ifdef DEBUG
	if(BmpWorker_InfoHeader_read(fichier, imgHeader) != SUCCESS)
	{
		puts("Impossible d'ouvrir le header BMP.");
	}
  	puts("Header BMP ouvert lu avec succès.");
	#endif

    uint32_t width = imgHeader->img_width;
    uint32_t height = imgHeader->img_height;

	#ifdef DEBUG
		printf("W: %i, H: %i, TS: %i, RS: %i\n", width, height, width * height, imgHeader->bmp_bytesz);
	#endif


	//Lecture des données de l'image
    uint8_t * pData = (uint8_t *) malloc(imgHeader->bmp_bytesz * sizeof(uint8_t));

    #ifdef DEBUG
    if(BmpWorker_RawData_read(fichier, fileHeader, imgHeader, pData) != SUCCESS)
    {
    	puts("Impossible de lire les données de l'image.");
    }
	#endif


    // Conversion
    int i, j;
    int image[height][width];

    for(i = 0; i < height; i++)
    {
    	for(j = 0; j < width; j++)
    	{
    		image[i][j] = (int) *(pData + i * width * 3 + j * 3);
    		printf("%d\n", image[i][j]);
    	}
    }


	fclose(fichier);
	return(image);
}

int main(void)
{
	int **image;
	image = (int *) malloc()

	image = get_image_pixels("/home/fra_592/Documents/Cours/2017/Projet_Algo/BMP/img/test_32x32.bmp");
}
