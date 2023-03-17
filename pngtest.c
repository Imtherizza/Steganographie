#include "pngwrap.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
//#include "lib/bitstream.h"
//#include "lib/message.h"
#include "lib/stegano.h"

int main(int argc, char** argv)
{
  FILE *pfile;
  pfile = fopen("../pnghex.txt","wt");
  if(pfile==NULL) exit(0);

  bwimage_t *image;
  error_e retval=E3A_OK;
  image=E3ACreateImage();

  unsigned char *image_data;
  unsigned char *msg_data;
  size_t msg_size;
  unsigned int position = 0;
  unsigned int capacity = 0;
  
  do
  {
    unsigned int i = 0,j=0;
    if(E3A_OK != (retval=E3ALoadImage("../lena.png", image))) break;

    /* Test of file */
    image_data = malloc(sizeof(char)*image->width*image->height);
    if(argv[1] != NULL)
    {
      msg_size = STEG_extract_file(msg_data,argv[1]);

      // Just to do something - print in pnghex 
      if(1)
      {
        for(i=0; i<msg_size; i++)
        {
          fprintf(pfile,"%x ",msg_data[i]);
        }
        // print debug
        if(1)
        {
          for(int mdr=0;mdr<40;mdr++)
          {
              printf("%x\n",msg_data[mdr]);
          }
        }
      }

      capacity = STEG_est_max_in_img(image_data,image->width*image->height,NO_COLOR);
      printf("\n%ld\n",capacity);
      
      if(capacity>msg_size)
      {
        while(position<capacity)
        {
          STEG_write_bit_LSB(image_data,msg_data[j],&position);
          j++;
        }
      }
      STEG_process_GRAY_linear(image_data,image);
    }

    if(E3A_OK != (retval=E3ADumpImage("negative.png", image))) break;
    printf("%dx%d\n", image->width, image->height);
  }
  while(0);

  switch(retval){
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
  fclose(pfile);

  return 0;
}