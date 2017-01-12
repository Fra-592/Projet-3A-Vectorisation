#include "BMP/bmp_worker.h"
#include "io.h"

#define DEBUG

int main(void)
{
  	// Ouverture du fichier
	FILE * fBMP = fopen("/home/fra_592/Documents/Cours/2017/Projet_Algo/BMP/img/test_32x32.bmp", "rb+");
	if(fBMP == NULL)
	{
		#ifdef DEBUG
			puts("Can't open the BMP file");
		#endif
		return(ERROR_OPENING_FILE);
	}
	#ifdef DEBUG
    	puts("File opened.");
	#endif

	// Lecture des Headers
    BmpWorker_fileHeader * fileHeader = (BmpWorker_fileHeader *) malloc(sizeof(BmpWorker_fileHeader));
    BmpWorker_infoHeader * bmpHeader = (BmpWorker_infoHeader *) malloc(sizeof(BmpWorker_infoHeader));
    
    if(BmpWorker_FileHeader_read(fBMP, fileHeader) != SUCCESS)
    {
    	#ifdef DEBUG
			puts("Can't read file header.");
		#endif
    	return(ERROR_INVALID_FILE_HEADER);
    }
    #ifdef DEBUG
    	puts("File header opened.");
	#endif

	if(BmpWorker_InfoHeader_read(fBMP, bmpHeader) != SUCCESS)
	{
		#ifdef DEBUG
			puts("Can't read info header.");
		#endif
		return(ERROR_INVALID_INFO_HEADER);
	}
	#ifdef DEBUG
    	puts("Info header opened.");
	#endif

    uint32_t width = bmpHeader->img_width;
    uint32_t height = bmpHeader->img_height;
    uint8_t * pData = (uint8_t *) malloc(bmpHeader->bmp_bytesz * sizeof(uint8_t));
	
	#ifdef DEBUG
		printf("W: %i, H: %i, TS: %i, RS: %i\n", width, height, width * height, bmpHeader->bmp_bytesz);
	#endif


	// Lecture des données
    if(BmpWorker_RawData_read(fBMP, fileHeader, bmpHeader, pData) != SUCCESS)
    {
    	#ifdef DEBUG
    		puts("Can't read bitmap data.");
		#endif
    	return(ERROR_INVALID_DATA);
    }

    int i, j;
    int image[height][width];

    // Conversion
    for(i = 0; i < height; i++)
    {
    	for(j = 0; j < width; j++)
    	{
    		image[i][j] = (int) *(pData + i * width * 3 + j * 3);
    		printf("%d\n", image[i][j]);
    	}
    }
}