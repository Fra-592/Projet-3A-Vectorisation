/*
 * bmp_worker.c
 *
 *  Created on: 2011-07-10
 *      Author: Thomas Bores
 */

#include "bmp_worker.h"

uint32_t
BmpWorker_FileHeader_size()
{
  return sizeof(uint16_t) * 3 + sizeof(uint32_t) * 2;
}

void
BmpWorker_FileHeader_display(const BmpWorker_fileHeader * fileHeader)
{
  fprintf(stdout, "*** BMP File Header ***\n");
  fprintf(stdout, "signature:   0x%.4X\n", fileHeader->signature);
  fprintf(stdout, "fileSize:    0x%.8X (%d bytes)\n", fileHeader->fileSize,
      fileHeader->fileSize);
  fprintf(stdout, "reserved_1:  0x%.4X\n", fileHeader->reserved_1);
  fprintf(stdout, "reserved_2:  0x%.4X\n", fileHeader->reserved_2);
  fprintf(stdout, "data_offset: 0x%.8X, %d\n", fileHeader->data_offset,
      fileHeader->data_offset);
  fprintf(stdout, "***********************\n");
  return;
}

int8_t
BmpWorker_FileHeader_read(FILE * fBMP, BmpWorker_fileHeader * fileHeader)
{
  int8_t retValue = ERROR_INPUT_PARAMETER_NULL;

  if (fBMP != NULL && fileHeader != NULL)
  {
    fseek(fBMP, 0, SEEK_SET);

    fread(&(fileHeader->signature), sizeof(uint16_t), 1, fBMP);

    // A valid windows BMP file starts with "BM"
    if (fileHeader->signature == 0x4D42)
    {
      fread(&(fileHeader->fileSize), sizeof(uint32_t), 1, fBMP);
      fread(&(fileHeader->reserved_1), sizeof(uint16_t), 1, fBMP);
      fread(&(fileHeader->reserved_2), sizeof(uint16_t), 1, fBMP);
      fread(&(fileHeader->data_offset), sizeof(uint32_t), 1, fBMP);
      retValue = SUCCESS;
    }
    else
    {
      retValue = ERROR_INVALID_FILE_HEADER;
    }
  }

  return retValue;
}

int8_t
BmpWorker_FileHeader_write(FILE * fBMP, const BmpWorker_fileHeader * fileHeader)
{
  int8_t retValue = ERROR_INPUT_PARAMETER_NULL;

  if (fBMP != NULL && fileHeader != NULL)
  {
    fseek(fBMP, 0, SEEK_SET);

    // A valid windows BMP file starts with "BM"
    if (fileHeader->signature == 0x4D42)
    {
      fwrite(&(fileHeader->signature), sizeof(uint16_t), 1, fBMP);
      fwrite(&(fileHeader->fileSize), sizeof(uint32_t), 1, fBMP);
      fwrite(&(fileHeader->reserved_1), sizeof(uint16_t), 1, fBMP);
      fwrite(&(fileHeader->reserved_2), sizeof(uint16_t), 1, fBMP);
      fwrite(&(fileHeader->data_offset), sizeof(uint32_t), 1, fBMP);
      retValue = SUCCESS;
    }
    else
    {
      retValue = ERROR_INVALID_FILE_HEADER;
    }
  }

  return retValue;
}

void
BmpWorker_InfoHeader_display(const BmpWorker_infoHeader * infoHeader)
{
  fprintf(stdout, "*** BMP Info Header ***\n");
  fprintf(stdout, "size: 0x%.8X (%d bytes)\n", infoHeader->size,
      infoHeader->size);
  fprintf(stdout, "img_width: %d pixels\n", infoHeader->img_width);
  fprintf(stdout, "img_height: %d pixels\n", infoHeader->img_height);
  fprintf(stdout, "nplanes: %d color planes (must be 1)\n", infoHeader->nplanes);
  fprintf(stdout, "bitspp: %d bits/pixels\n", infoHeader->bitspp);
  fprintf(stdout, "compress_type: 0x%.8X (%d)\n", infoHeader->compress_type,
      infoHeader->compress_type);
  fprintf(stdout, "bmp_bytesz: %d bytes\n", infoHeader->bmp_bytesz);
  fprintf(stdout, "hres: %d pixels\n", infoHeader->hres);
  fprintf(stdout, "vres: %d pixels\n", infoHeader->vres);
  fprintf(stdout, "ncolors: %d colors\n", infoHeader->ncolors);
  fprintf(stdout, "nimpcolors: %d colors\n", infoHeader->nimpcolors);
  fprintf(stdout, "***********************\n");
  return;
}

int8_t
BmpWorker_InfoHeader_read(FILE * fBMP, BmpWorker_infoHeader * infoHeader)
{
  int8_t retValue = ERROR_INPUT_PARAMETER_NULL;

  if (fBMP != NULL)
  {
    fseek(fBMP, BmpWorker_FileHeader_size(), SEEK_SET);

    fread(&(infoHeader->size), sizeof(uint32_t), 1, fBMP);
    fread(&(infoHeader->img_width), sizeof(int32_t), 1, fBMP);
    fread(&(infoHeader->img_height), sizeof(int32_t), 1, fBMP);
    fread(&(infoHeader->nplanes), sizeof(uint16_t), 1, fBMP);
    fread(&(infoHeader->bitspp), sizeof(uint16_t), 1, fBMP);
    fread(&(infoHeader->compress_type), sizeof(uint32_t), 1, fBMP);
    fread(&(infoHeader->bmp_bytesz), sizeof(uint32_t), 1, fBMP);
    fread(&(infoHeader->hres), sizeof(int32_t), 1, fBMP);
    fread(&(infoHeader->vres), sizeof(int32_t), 1, fBMP);
    fread(&(infoHeader->ncolors), sizeof(uint32_t), 1, fBMP);
    fread(&(infoHeader->nimpcolors), sizeof(uint32_t), 1, fBMP);

    // Sometimes bmp_bytesz is set to 0 in the BMP file.
    // Yes, I have seen such example of files saved with MS Paint :-S.
    // In that case, we calculate the bmp data size by ourselves
    if (infoHeader->bmp_bytesz == 0)
    {
      if (infoHeader->img_width != 0 && infoHeader->img_height != 0
          && infoHeader->bitspp != 0)
      {
        uint32_t depth = infoHeader->bitspp / 8;
        uint32_t nBytesPerLine = infoHeader->img_width * depth;

        // Standard says that the number of bytes for each line
        // must be a multiple of 4. If not, padding bytes are added.
        // So we calculate, if any, how many padding bytes are present.
        while (nBytesPerLine % 4 != 0)
        {
          nBytesPerLine++;
        }
        infoHeader->bmp_bytesz = nBytesPerLine * infoHeader->img_height;
        retValue = SUCCESS;
      }
      else
      {
        retValue = ERROR_INVALID_FILE_HEADER;
      }
    }
    else
    {
      retValue = SUCCESS;
    }
  }
  return retValue;
}

int8_t
BmpWorker_InfoHeader_write(FILE * fBMP, const BmpWorker_infoHeader * infoHeader)
{
  int8_t retValue = ERROR_INPUT_PARAMETER_NULL;

  if (fBMP != NULL)
  {
    fseek(fBMP, BmpWorker_FileHeader_size(), SEEK_SET);

    fwrite(&(infoHeader->size), sizeof(uint32_t), 1, fBMP);
    fwrite(&(infoHeader->img_width), sizeof(int32_t), 1, fBMP);
    fwrite(&(infoHeader->img_height), sizeof(int32_t), 1, fBMP);
    fwrite(&(infoHeader->nplanes), sizeof(uint16_t), 1, fBMP);
    fwrite(&(infoHeader->bitspp), sizeof(uint16_t), 1, fBMP);
    fwrite(&(infoHeader->compress_type), sizeof(uint32_t), 1, fBMP);
    fwrite(&(infoHeader->bmp_bytesz), sizeof(uint32_t), 1, fBMP);
    fwrite(&(infoHeader->hres), sizeof(int32_t), 1, fBMP);
    fwrite(&(infoHeader->vres), sizeof(int32_t), 1, fBMP);
    fwrite(&(infoHeader->ncolors), sizeof(uint32_t), 1, fBMP);
    fwrite(&(infoHeader->nimpcolors), sizeof(uint32_t), 1, fBMP);

    retValue = SUCCESS;
  }
  return retValue;
}

int8_t
BmpWorker_RawData_read(FILE * fBMP, const BmpWorker_fileHeader * fileHeader,
                       const BmpWorker_infoHeader * infoHeader, uint8_t * pData)
{
  int8_t retValue = ERROR_INPUT_PARAMETER_NULL;

  // LECTURE DONNÉES?
  if (fBMP != NULL && fileHeader && infoHeader != NULL)
  {
    uint32_t nBytesRead = 0;
    fseek(fBMP, fileHeader->data_offset, SEEK_SET);

    nBytesRead = fread(pData, 1, infoHeader->bmp_bytesz, fBMP);

    if (nBytesRead == infoHeader->bmp_bytesz)
    {
      retValue = SUCCESS;
    }
    else
    {
      if (feof(fBMP) != 0)
      {
        // Well, it shouldn't happen.
        // If you reach this point, verify that you have really opened
        // your bitmap file in binary mode.
        fprintf(stderr, "EOF: Bytes read = %d\n", nBytesRead);
        fprintf(stderr, "EOF: Attended bytes = %d\n", infoHeader->bmp_bytesz);
        retValue = SUCCESS;
      }

      if (ferror(fBMP) != 0)
      {
        fprintf(stderr, "FERROR: Bytes read = %d\n", nBytesRead);
        fprintf(stderr, "FERROR: Attended bytes = %d\n", infoHeader->bmp_bytesz);
        retValue = ERROR_INVALID_DATA;
      }
    }
  }
  return retValue;
}

void
BmpWorker_RawData_display(const BmpWorker_infoHeader * infoHeader,
                          const uint8_t * pData)
{
  if (pData != NULL && infoHeader != NULL)
  {
    uint32_t pixelCounter = 0;
    uint32_t nPixels = infoHeader->img_width * infoHeader->img_height;
// LECTURE DONNÉES?
    for (pixelCounter = 0; pixelCounter < nPixels; pixelCounter++)
    {
      uint8_t byteCounter = 0;
      uint16_t nBytesPerPixel = infoHeader->bitspp / 8;

      fprintf(stdout, "Pixel %.6d, color #", pixelCounter);

      for (byteCounter = 0; byteCounter < nBytesPerPixel; byteCounter++)
      {
        fprintf(stdout, "%.2X", *pData);
        pData++;
      }
      fprintf(stdout, "\n");
    }
  }
}

int8_t
BmpWorker_RawData_white2black(const BmpWorker_infoHeader * infoHeader,
                              uint8_t * pData)
{
  int8_t retValue = ERROR_INPUT_PARAMETER_NULL;

  if (pData != NULL && infoHeader != NULL)
  {
    uint32_t pixelCounter = 0;
    uint32_t nPixels = infoHeader->img_width * infoHeader->img_height;

    for (pixelCounter = 0; pixelCounter < nPixels; pixelCounter++)
    {
      uint8_t byteCounter = 0;
      uint16_t nBytesPerPixel = infoHeader->bitspp / 8;
      int8_t white = 0;

      for (byteCounter = 0; byteCounter < nBytesPerPixel; byteCounter++)
      {
        if (*pData != 0xFF)
        {
          white = -1;
        }

        pData++;
      }

      // If pixel is white, convert it in a black one.
      if (white == 0)
      {
        pData -= nBytesPerPixel;

        for (byteCounter = 0; byteCounter < nBytesPerPixel; byteCounter++)
        {
          *pData = 0x00;
          pData++;
        }
      }
    }
    retValue = SUCCESS;
  }
  return retValue;
}

void
BmpWorker_RawData_write(FILE * fBMP, const BmpWorker_fileHeader * fileHeader,
                        const BmpWorker_infoHeader * infoHeader,
                        const uint8_t * pData)
{
  fseek(fBMP, fileHeader->data_offset, SEEK_SET);
  fwrite(pData, sizeof(uint8_t), infoHeader->bmp_bytesz, fBMP);
}

uint8_t *
BmpWorker_RawData_invertLines(const uint8_t *pData, const uint32_t nLines,
                              const uint32_t bytes_per_line)
{
  uint8_t * outData = NULL;

  if (pData != NULL)
  {
    uint32_t i = 0;
    uint32_t j = 0;
    uint32_t lCounter;

    uint32_t dataSize = nLines * bytes_per_line;
    uint32_t offset = dataSize;

    uint8_t * line = (uint8_t *) malloc(bytes_per_line * sizeof(uint8_t));

    if (line != NULL)
    {
      outData = (uint8_t *) malloc(dataSize * sizeof(uint8_t));

      if (outData != NULL)
      {
        for (lCounter = 0; lCounter < nLines; lCounter++)
        {
          // Read current line
          for (i = 0; i < bytes_per_line; i++)
          {
            line[i] = pData[i + lCounter * bytes_per_line];
          }

          // Once line is ready, copy it to the right location in outData
          for (i = bytes_per_line, j = 0; j < bytes_per_line; i--, j++)
          {
            outData[offset - i] = line[j];
          }
          offset -= bytes_per_line;
        }
      }
      free(line);
    }
  }

  return outData;
}

uint8_t *
BmpWorker_RawData_invertRBValues(const uint8_t * pData,
                                 const uint32_t pData_size)
{
  uint8_t * newData = NULL;

  if (pData != NULL)
  {
    newData = (uint8_t *) malloc(pData_size * sizeof(uint8_t));

    if (newData != NULL)
    {
      uint8_t i = 0;
      uint32_t j = 0;
      uint8_t pixel[3] =
      { 0, 0, 0 };
      uint8_t invertedPixel[3] =
      { 0, 0, 0 };

      while (j < pData_size)
      {
        // Read pixel
        for (i = 0; i < 3; i++)
        {
          pixel[i] = pData[j + i];
        }

        // Invert RGB to BGR
        for (i = 0; i < 3; i++)
        {
          invertedPixel[2 - i] = pixel[i];
        }

        // Write it
        for (i = 0; i < 3; i++)
        {
          newData[j + i] = invertedPixel[i];
        }

        j += 3;
      }
    }
  }
  return newData;
}

uint8_t *
BmpWorker_RawData_make4BytesCompatible(const uint8_t * pData,
                                       const uint32_t pData_size)
{
  uint8_t * bmpData = NULL;

  if (pData != NULL)
  {
    uint32_t i, j, z;
    uint32_t bmpDataSize = (pData_size + pData_size / 3) * sizeof(uint8_t);
    bmpData = (uint8_t *) malloc(bmpDataSize);

    if (bmpData != NULL)
    {
      for (i = 0, j = 0, z = 0; i < bmpDataSize; i++)
      {
        switch (j)
        {
          case 0: // B
            bmpData[i] = pData[z];
            z++;
            break;
          case 1: // G
            bmpData[i] = pData[z];
            z++;
            break;
          case 2: // R
            bmpData[i] = pData[z];
            z++;
            break;
          case 3: // Set to 0xFF for cases where this value will be considered for alpha
            bmpData[i] = 0xFF;
            j = -1;
            break;
        }
        j++;
      }
    }
  }
  return bmpData;
}

uint8_t *
BmpWorker_RawData_removeEndlinePaddingBytes(const uint8_t * pData,
                                            const BmpWorker_infoHeader * infoHeader)
{
  uint8_t * outData = NULL;

  if (pData != NULL && infoHeader != NULL)
  {
    uint32_t depth = infoHeader->bitspp / 8;
    uint32_t nBytesPerLine = infoHeader->img_width * depth;
    uint32_t nPaddingPerLine = 0;

    uint32_t lineCounter = 0;
    uint32_t byteCounter = 0;

    // Calculate padding
    while (nBytesPerLine % 4 != 0)
    {
      nBytesPerLine++;
      nPaddingPerLine++;
    }

    outData = (uint8_t *) malloc(sizeof(uint8_t) * (nBytesPerLine
        - nPaddingPerLine) * infoHeader->img_height);

    // Read each line of pData and put it in outData but without padding
    for (lineCounter = 0; lineCounter < infoHeader->img_height; lineCounter++)
    {
      for (byteCounter = 0; byteCounter < (nBytesPerLine - nPaddingPerLine); byteCounter++)
      {
        outData[lineCounter * (nBytesPerLine - nPaddingPerLine) + byteCounter]
            = pData[lineCounter * nBytesPerLine + byteCounter];
      }
    }
  }
  return outData;
}

int8_t
BmpWorker_saveOnDisk(char * fileName, const BmpWorker_fileHeader * fileHeader,
                     const BmpWorker_infoHeader * infoHeader,
                     const uint8_t * pData)
{
  int8_t retValue = ERROR_CLOSING_FILE;

  if (fileName != NULL && fileHeader != NULL && infoHeader != NULL && pData
      != NULL)
  {
    FILE * fBMP = fopen(fileName, "wb");

    if (fBMP != NULL)
    {
      uint32_t depth = infoHeader->bitspp / 8;
      uint32_t nBytesPerLine = depth * infoHeader->img_width;
      uint8_t nPaddingBytes = 0;

      // Save the file header
      BmpWorker_FileHeader_write(fBMP, fileHeader);

      // Save the information header
      BmpWorker_InfoHeader_write(fBMP, infoHeader);

      // Save the bitmap data

      // Calculate the number of padding bytes needed.
      while ((nBytesPerLine + nPaddingBytes) % 4 != 0)
      {
        nPaddingBytes++;
      }

      if (nPaddingBytes == 0)
      {
        BmpWorker_RawData_write(fBMP, fileHeader, infoHeader, pData);
      }
      else
      {
        // We must add the padding byte to each line.
        uint8_t * outData = (uint8_t *) malloc(depth * infoHeader->img_width
            * infoHeader->img_height + nPaddingBytes * infoHeader->img_height);

        if (outData != NULL)
        {
          uint32_t lCounter = 0;
          uint32_t byteCounter = 0;
          uint32_t paddingCounter = 0;

          // For each line
          for (lCounter = 0; lCounter < infoHeader->img_height; lCounter++)
          {
            // Copy the line content
            for (byteCounter = 0; byteCounter < nBytesPerLine; byteCounter++)
            {
              outData[lCounter * (nBytesPerLine + nPaddingBytes) + byteCounter]
                  = pData[lCounter * nBytesPerLine + byteCounter];
            }

            // Add the padding bytes
            for (paddingCounter = 0; paddingCounter < nPaddingBytes; paddingCounter++)
            {
              outData[lCounter * (nBytesPerLine + nPaddingBytes) + byteCounter
                  + paddingCounter] = 0x00;
            }
          }
        }
        else
        {
          retValue = ERROR_NOT_ENOUGH_MEMORY;
        }

        BmpWorker_RawData_write(fBMP, fileHeader, infoHeader, outData);
      }
      fclose(fBMP);
    }
    else
    {
      retValue = ERROR_OPENING_FILE;
    }
  }

  return retValue;
}
