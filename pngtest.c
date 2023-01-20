#include "pngwrap.h"
#include "lib/bitstream.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv)
{
  bitstream image_bs;
  bwimage_t *image;
  error_e retval=E3A_OK;

  image=E3ACreateImage();

  do
  {
    unsigned int i, j;//, tamer;

    if(E3A_OK != (retval=E3ALoadImage(argv[1], image))) break;
    image_bs.data = malloc(sizeof(short)*image->width*image->height*3);

    /* Just to do something - create a negative */
    for(i=0; i<image->height; i++)
    {
      for(j=0; j<image->width; j++)
      {
        image->data[i][j]=0xff-image->data[i][j];
        image_bs.data[i+j*image->width]=image->data[i][j];
        //printf("%d\n",tamer);
        //tamer++;
      }
    }

    if(E3A_OK != (retval=E3ADumpImage("positive.png", image))) break;

    printf("%dx%d\n", image->width, image->height);
    printf("%ld\n",sizeof(short)*256);
  }
  while(0);

  switch(retval)
  {
    case E3A_OK:
      break;
    case E3A_ENOFILE:
      fprintf(stderr, "Cannot open file\n");
      break;
    case E3A_EPNGLIB:
      fprintf(stderr, "Internal error of libpng\n");
      break;
    case E3A_ENOMEM:
      fprintf(stderr, "Cannot allocate memory\n");
      break;
    default:
      ;/* Can't happen */
  }
  E3AFreeImage(image);
  //free(image_bs.data);

  return 0;
}