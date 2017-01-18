#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

#include "bmp_worker.h"
#include "io.h"

void list_images(void)
{
	char dirname[] = "img";
	DIR *imgDir = opendir(dirname);

	chdir("img");
	struct dirent *file = NULL;

	printf("Images disponibles:\n");

	while((file = readdir(imgDir)) != NULL)
	{
		if(strcmp(file->d_name, ".") && strcmp(file->d_name, ".."))
		{
			printf("%s, dimensions: %d x %d,\n", file->d_name,
					get_image_width(file->d_name),
					get_image_height(file->d_name));
		}
	}
	closedir(imgDir);
}


int get_image_width(const char *imgName)
{
	FILE *imgFile = fopen(imgName, "rb+");
	BmpWorker_infoHeader *infoHeader = (BmpWorker_infoHeader *) malloc(sizeof(BmpWorker_infoHeader));
	
	BmpWorker_InfoHeader_read(imgFile, infoHeader);
	int imgWidth = infoHeader->img_width;

	//Libération de mémoire
	free(infoHeader);
	fclose(imgFile);
	return(imgWidth);
}


int get_image_height(const char *imgName)
{
	FILE *imgFile = fopen(imgName, "rb+");
	BmpWorker_infoHeader *infoHeader = (BmpWorker_infoHeader *) malloc(sizeof(BmpWorker_infoHeader));
	
	BmpWorker_InfoHeader_read(imgFile, infoHeader);
	int imgHeight = infoHeader->img_height;

	//Libération de mémoire
	free(infoHeader);
	fclose(imgFile);
	return(imgHeight);
}


int is_valid_img(const char *imgName)
{
	FILE *imgFile = fopen(imgName, "rb+");
	if(!imgFile)
	{
		return(0);
	}

    BmpWorker_fileHeader *fileHeader = (BmpWorker_fileHeader *) malloc(sizeof(BmpWorker_fileHeader));
	BmpWorker_infoHeader *infoHeader = (BmpWorker_infoHeader *) malloc(sizeof(BmpWorker_infoHeader));
	
	if(BmpWorker_FileHeader_read(imgFile, fileHeader))
	{
		return(0);
	}

	if(BmpWorker_InfoHeader_read(imgFile, infoHeader))
	{
		return(0);
	}

    uint8_t *pData = (uint8_t *) malloc(infoHeader->bmp_bytesz * sizeof(uint8_t));
    
    if(BmpWorker_RawData_read(imgFile, fileHeader, infoHeader, pData))
    {
    	return(0);
    }

    //Libération de mémoire
    free(pData);
    free(fileHeader);
    free(infoHeader);
    fclose(imgFile);
    return(1);
}


void get_image_pixels(const char *imgName, int **image)
{
	FILE *imgFile = fopen(imgName, "rb+");

	//Lecture des headers
    BmpWorker_fileHeader *fileHeader = (BmpWorker_fileHeader *) malloc(sizeof(BmpWorker_fileHeader));
	BmpWorker_infoHeader *infoHeader = (BmpWorker_infoHeader *) malloc(sizeof(BmpWorker_infoHeader));
	
	BmpWorker_FileHeader_read(imgFile, fileHeader);
	BmpWorker_InfoHeader_read(imgFile, infoHeader);

    uint32_t imgWidth = infoHeader->img_width;
    uint32_t imgHeight = infoHeader->img_height;

	//Lecture des données de l'image
    uint8_t *pData = (uint8_t *) malloc(infoHeader->bmp_bytesz * sizeof(uint8_t));

    BmpWorker_RawData_read(imgFile, fileHeader, infoHeader, pData);

    // Conversion
    int i, j;

    for(i = 0; i < imgHeight; i++)
    {
    	for(j = 0; j < imgWidth; j++)
    	{
    		image[imgHeight-i-1][j] = *(pData + i * imgWidth * 3 + j * 3);
    		image[imgHeight-i-1][j] = image[imgHeight-i-1][j] | (*(pData + i * imgWidth * 3 + j * 3 + 1) << 8);
    		image[imgHeight-i-1][j] = image[imgHeight-i-1][j] | (*(pData + i * imgWidth * 3 + j * 3 + 2) << 16);
    		image[imgHeight-i-1][j] = (image[imgHeight-i-1][j] != 0xFFFFFF);
    	}
    }

    //Libération de mémoire
    free(infoHeader);
    free(fileHeader);
    free(pData);
	fclose(imgFile);
}
