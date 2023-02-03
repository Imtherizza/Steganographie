#include "pngwrap.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "lib/bitstream.h"
#include "lib/message.h"
#include "lib/stegano.h"

//not do this
//

int main(){
  bitstream *image_bs = CreateEmptyBitstream();
  bitstream *fake_data = CreateEmptyBitstream();

  unsigned char *image_data;
  unsigned int image_data_size;
  bwimage_t *image;
  error_e retval=E3A_OK;

  FILE *t_image,*t_image2;
  t_image = fopen("../pnghex.txt","wt");
  t_image2 = fopen("../pnghex2.txt","wt");

  image=E3ACreateImage();

  //tests on a single black & white image, tests on a rgb image possible
  do{
    unsigned int i, j,cpt=0;
    if(E3A_OK != (retval=E3ALoadImage("../lena.png", image))) break;

    /* Just to do something - create a copy of image */
    image_data = malloc(sizeof(char)*image->height*image->width);
    image_data_size = image->height*image->width;
    
    for(i=0; i<image->height; i++){
      for(j=0; j<image->width; j++){
        image_data[i+j*image->width]=image->data[i][j];
        fprintf(t_image,"%d ",image_data[i+j*image->width]);
        cpt+=sizeof(char);
      }
    }

    /* TEST : temporary stengano*/



    /* TEST : putting the entire image into the bitstream */
    image_bs = CreateBitstreamOnData(image_data,image_data_size);
    
    //fprint of bitstream data 
    for(i=0; i<image->height; i++){
      for(j=0; j<image->width; j++){
        fprintf(t_image2,"%d ",image_bs->data[i+j*image->width]);
      }
    }

    if(E3A_OK != (retval=E3ADumpImage("positive.png", image))) break;

    /* Debugging print */
    printf("pnghex size : %ld\n",sizeof(t_image));
    printf("size data: %d\n",image_data_size);
    printf("size image_bs : %d\n",image_bs->size);
    printf("image_bs capacity : %d\n", image_bs->capacity);
    printf("\nimg size : %dx%d\n", image->width, image->height);
    printf("check if cpt == nb of bits manipulated, cpt : %ld --- %d \n",sizeof(char)*image->width*image->height,cpt);
  }
  while(0);

  //int ccrc = CRC_calc(image_bs->data,image_data_size,13);
  //printf("CRC : %d\n",ccrc);


  /* Error status check*/
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