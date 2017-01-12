/*
 * main.c
 *
 *  Created on: 2011-07-10
 *      Author: Thomas Bores
 */

#include <stdio.h>
#include <stdlib.h>

#include "bmp_worker.h"

// Define X11 to be able to display your BMP images
// in a XWindow. Works only on system where X11 is installed.
//#define X11 // Comment this line, if you haven't X11 installed.
#ifdef X11
#include <Xlib.h>
GC gc;
Display *display;
Visual *visual;
int screen;
int nplanes;
Window win, root;
unsigned long white_pixel, black_pixel;
unsigned int visdepth;
char *dpyn = NULL;
XImage * xImg = NULL;

void
expose(XImage * xImg, unsigned int width, unsigned int height)
{
  XPutImage(display, win, gc, xImg, 0, 0, 0, 0, width, height);
}
#endif

int
main(void)
{
  puts("BmpWorker started.");

    FILE * fBMP = fopen("/home/thomas/workspace/bmpworker/img/test_240x240.bmp",
        "rb+");

  if (fBMP != NULL)
  {
    puts("File opened.");

    BmpWorker_fileHeader * fileHeader = (BmpWorker_fileHeader *) malloc(
        sizeof(BmpWorker_fileHeader));

    BmpWorker_infoHeader * bmpHeader = (BmpWorker_infoHeader *) malloc(
        sizeof(BmpWorker_infoHeader));

    if (BmpWorker_FileHeader_read(fBMP, fileHeader) == SUCCESS)
    {
      BmpWorker_FileHeader_display(fileHeader);

      if (BmpWorker_InfoHeader_read(fBMP, bmpHeader) == SUCCESS)
      {
        uint32_t width = bmpHeader->img_width;
        uint32_t height = bmpHeader->img_height;

        uint8_t * pData = (uint8_t *) malloc(bmpHeader->bmp_bytesz
            * sizeof(uint8_t));

        BmpWorker_InfoHeader_display(bmpHeader);
        if (BmpWorker_RawData_read(fBMP, fileHeader, bmpHeader, pData)
            == SUCCESS)
        {
          // Uncomment for testing following functions
          // BmpWorker_RawData_display(bmpHeader, pData);
          // BmpWorker_RawData_white2black(bmpHeader, pData);
          // BmpWorker_RawData_display(bmpHeader, pData);
          // BmpWorker_SaveOnDisk("/home/thomas/Images/test_32x32_new.bmp", fileHeader,
          //                      bmpHeader, pData);

          pData = BmpWorker_RawData_removeEndlinePaddingBytes(pData, bmpHeader);
          pData = BmpWorker_RawData_invertLines(pData, height, width
              * bmpHeader->bitspp / 8);


#ifdef X11
          if ((display = XOpenDisplay(dpyn)) != NULL)
          {
            gc = DefaultGC (display, screen);
            screen = DefaultScreen (display);
            visual = DefaultVisual(display, screen);
            visdepth = DefaultDepth(display, screen);
            nplanes = DisplayPlanes(display, screen);
            root = RootWindow (display, screen);
            white_pixel = WhitePixel (display, screen);
            black_pixel = BlackPixel (display, screen);

            win = XCreateSimpleWindow(display, root, 0, 0, width, height, 2,
                black_pixel, white_pixel);

            XSelectInput(display, win, ExposureMask);

            XStoreName(display, win, "BMP Viewer");
            XMapWindow(display, win);

            fprintf(stdout, "visdepth = %d\n", visdepth);
            fprintf(stdout, "nplanes = %d\n", nplanes);
            fprintf(stdout, "BitmapPad(display) = %d\n", BitmapPad(display));

            pData = BmpWorker_RawData_make4BytesCompatible(pData,
                bmpHeader->bmp_bytesz);

            xImg = XCreateImage(display, visual, visdepth, ZPixmap, 0,
                (char *) pData, width, height, BitmapPad(display), 0);

            // Uncomment to see information about the xImage
//            fprintf(stdout, "***********\n");
//            fprintf(stdout, "xImg->bitmap_bit_order: %d\n", xImg->bitmap_bit_order);
//            fprintf(stdout, "xImg->bitmap_pad: %d\n", xImg->bitmap_pad);
//            fprintf(stdout, "xImg->bitmap_unit: %d\n", xImg->bitmap_unit);
//            fprintf(stdout, "xImg->bits_per_pixel: %d\n", xImg->bits_per_pixel);
//            fprintf(stdout, "xImg->bytes_per_line: %d\n", xImg->bytes_per_line);
//            fprintf(stdout, "xImg->depth: %d\n", xImg->depth);
//            fprintf(stdout, "***********\n");

#ifdef __i386__
            xImg->byte_order = LSBFirst;
#else
            xImg->byte_order = MSBFirst;
#endif

            if (xImg != NULL)
            {
              for (;;)
              {
                XEvent ev;
                XNextEvent(display, &ev);

                switch (ev.type)
                {
                  case Expose:
                    expose(xImg, width, height);
                    break;

                  default:

                    break;
                }
              }
            }
            else
            {
              fprintf(stderr, "Error XCreateImage return NULL\n");
            }
          }
          else
          {
            fprintf(stderr, "Can't open Display\n");
          }
#endif // X11
        }
        else
        {
          fprintf(stderr, "Can't read bitmap data\n");
        }
        free(pData);
      }
      else
      {
        fprintf(stderr, "Can't read info header\n");
      }
    }
    else
    {
      fprintf(stderr, "Can't read file header.\n");
    }

    if (fclose(fBMP) != 0)
    {
      fprintf(stderr, "Cannot close BMP file.\n");
      return ERROR_CLOSING_FILE;
    }
  }
  else
  {
    fprintf(stderr, "Can't open the BMP file.\n");
  }

  puts("Quit.");

  return EXIT_SUCCESS;
}
