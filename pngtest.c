#include "pngwrap.h"
#include <stdio.h>
#include <stdlib.h>
#include "lib/bitstream.h"

int main(){
  bitstream *image_bs = CreateEmptyBitstream();
  unsigned short *image_data;
  unsigned int image_data_size;
  bwimage_t *image;
  error_e retval=E3A_OK;

  FILE *t_image,*t_image2;
  t_image = fopen("../pnghex.txt","wt");
  t_image2 = fopen("../pnghex2.txt","wt");

  image=E3ACreateImage();
  
  do{
    unsigned int i, j,cpt=0;
    if(E3A_OK != (retval=E3ALoadImage("../lena.png", image))) break;

    /* Just to do something - create a copy of image */
    image_data = malloc(sizeof(short)*image->height*image->width);
    image_data_size = image->height*image->width;
    
    for(i=0; i<image->height; i++){
      for(j=0; j<image->width; j++){
        image_data[i+j*image->width]=image->data[i][j];
        fprintf(t_image,"%o ",image_data[i+j*image->width]);
        cpt+=2;
      }
    }

    /* fprint of bitstream data */
    image_bs = CreateBitstreamOnData(image_data,image_data_size);
    image_bs->capacity=(sizeof(short)*image->width*image->height)/8;
    for(i=0; i<image->height; i++){
      for(j=0; j<image->width; j++){
        fprintf(t_image2,"%o ",image_bs->data[i+j*image->width]);
      }
    }

    if(E3A_OK != (retval=E3ADumpImage("positive.png", image))) break;

    /* Debugging print */
    printf("size data: %d\n",image_data_size);
    printf("size image_bs : %d\n",image_bs->size);
    printf("image_bs capacity : %d\n", image_bs->capacity);
    printf("\nimg size : %dx%d\n", image->width, image->height);
    printf("check if cpt == nb of bits manipulated, cpt : %ld --- %d \n",sizeof(short)*image->width*image->height,cpt);
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

  /* Free */
  E3AFreeImage(image);
  FreeBitstream(image_bs);
  fclose(t_image);
  fclose(t_image2);

  return 0;
}