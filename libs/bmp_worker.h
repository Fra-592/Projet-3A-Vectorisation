/*
 * bmp_worker.h
 *
 *  Created on: 2011-07-10
 *      Author: Thomas Bores
 */

#ifndef BMP_WORKER_H_
#define BMP_WORKER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define SUCCESS 0
#define ERROR_INPUT_PARAMETER_NULL -1
#define ERROR_INVALID_INFO_HEADER -3
#define ERROR_INVALID_FILE_HEADER -4
#define ERROR_INVALID_DATA -5
#define ERROR_OPENING_FILE -6
#define ERROR_CLOSING_FILE -7
#define ERROR_NOT_ENOUGH_MEMORY -8

// A complete description of the hearders can be found here:
// http://en.wikipedia.org/wiki/BMP_file_format

// File Header
typedef struct Bmp_File_Header
{
  uint16_t signature;
  uint32_t fileSize; // Size of the file
  uint16_t reserved_1;
  uint16_t reserved_2;
  uint32_t data_offset; // starting address, of the byte where the bitmap image data (Pixel Array) can be found
} BmpWorker_fileHeader;

// Information header
typedef struct Bmp_Info_Header
{
  uint32_t size; // the size of this header
  int32_t img_width; // the bitmap width in pixels
  int32_t img_height; // the bitmap height in pixels
  uint16_t nplanes; // the number of color planes being used. Must be set to 1
  uint16_t bitspp; // the number of bits per pixel, which is the color depth of the image. Typical values are 1, 4, 8, 16, 24 and 32
  uint32_t compress_type; // the compression method being used
  uint32_t bmp_bytesz; // the image size. This is the size of the raw bitmap data, and should not be confused with the file size
  int32_t hres; // the horizontal resolution of the image. (pixel per meter)
  int32_t vres; // the vertical resolution of the image. (pixel per meter)
  uint32_t ncolors; // the number of colors in the color palette, or 0 to default to 2n
  uint32_t nimpcolors; // the number of important colors used, or 0 when every color is important; generally ignored
} BmpWorker_infoHeader;

// Return the size of the file header (14 bytes)
uint32_t
BmpWorker_FileHeader_size();

// Display on the console the content of the File Header
void
BmpWorker_FileHeader_display(const BmpWorker_fileHeader * fileHeader);

// Read the File Header from the bitmap file and save it in the header parameter.
int8_t
BmpWorker_FileHeader_read(FILE * fBMP, BmpWorker_fileHeader * fileHeader);

// Write the File Head in the bitmap file.
int8_t
BmpWorker_FileHeader_write(FILE * fBMP,
                           const BmpWorker_fileHeader * fileHeader);

// Display on the standard output the content of the File Header
void
BmpWorker_InfoHeader_display(const BmpWorker_infoHeader * infoHeader);

// Read the Information Header from the bitmap file and save it in the
// header parameter
int8_t
BmpWorker_InfoHeader_read(FILE * fBMP, BmpWorker_infoHeader * infoHeader);

// Write the information in the bitmap file.
int8_t
BmpWorker_InfoHeader_write(FILE * fBMP,
                           const BmpWorker_infoHeader * infoHeader);

// Read the bitmap data and save it in the pData pointer
int8_t
BmpWorker_RawData_read(FILE * fBMP,
                       const BmpWorker_fileHeader * fileHeader,
                       const BmpWorker_infoHeader * infoHeader,
                       uint8_t * pData);

// Display on the console all the pixels of the bitmap data.
void
BmpWorker_RawData_display(const BmpWorker_infoHeader * infoHeader,
                          const uint8_t * pData);

// Change all the white pixels in pData in black ones
int8_t
BmpWorker_RawData_white2black(const BmpWorker_infoHeader * infoHeader,
                              uint8_t * pData);

// Save only the pData in the fBMP file
void
BmpWorker_RawData_write(FILE * fBMP, const BmpWorker_fileHeader * fileHeader,
                        const BmpWorker_infoHeader * infoHeader,
                        const uint8_t * pData);

// Read all the line of the pData.
// First line becomes the last one and so one...
// Return a pointer on the memory area containing the new data.
uint8_t *
BmpWorker_RawData_invertLines(const uint8_t *pData, const uint32_t nLines,
                              const uint32_t bytes_per_line);

// Add a fourth byte (0xFF) to each pixel. This is useful for making
// the bmp data content compatible with other image datatype
// e.g: XImage
// Return a pointer to the memory area containing the data
// with 4 bytes depth pixels.
uint8_t *
BmpWorker_RawData_make4BytesCompatible(const uint8_t * pData,
                                       const uint32_t pData_size);

// Some images have the pixel colors coded in another direction.
// This function invert the colors. From RGB to BGR.
// pData MUST have a 24bits (3 bytes) depth.
uint8_t *
BmpWorker_RawData_invertRBValues(const uint8_t * pData, const uint32_t pData_size);

// The bits representing the bitmap pixels are packed in rows.
// The size of each row is rounded up to a multiple of 4 bytes by padding.
uint8_t *
BmpWorker_RawData_removeEndlinePaddingBytes(const uint8_t * pData,
                                    const BmpWorker_infoHeader * infoHeader);

// Write the all data in a file named fileName
int8_t
BmpWorker_saveOnDisk(char * fileName, const BmpWorker_fileHeader * fileHeader,
                     const BmpWorker_infoHeader * infoHeader,
                     const uint8_t * pData);

#endif /* BMP_WORKER_H_ */
