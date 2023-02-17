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
  
  do
  {
    unsigned int i, j;
    if(E3A_OK != (retval=E3ALoadImage("../lena.png", image))) break;

    /* Test of file */
    /*
    image_data = malloc(sizeof(char)*image->width*image->height);
    if(argv[1] != NULL)
    {
      int s;
      msg_size = STEG_extract_file(msg_data,argv[1]);

      if(STEG_est_max_in_img(image_data,image->width*image->height,NO_COLOR)>msg_size)
      {
        for(s=0; s<(image->width*image->height)-1;s++)
        {
          image_data[s] = image->rawdata[s];
          if(s<msg_size)
          {
            STEG_write_bit(image_data,msg_data[s],s*8);
          }
        }
      }
    }
    STEG_process_GRAY_linear(image_data,image);

    // Just to do something - print in pnghex 
    for(i=0; i<image->height; i++)
    {
      for(j=0; j<image->width; j++)
      {
        fprintf(pfile,"%x ",image->rawdata[i+j*image->width]);
      }
      fprintf(pfile,"\n");
    }
    */
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