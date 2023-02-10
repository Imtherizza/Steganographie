#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "lib/message.h"
#include "bitstream.h"
#include "lib/stegano.h"

void STEG_extract_file(unsigned char* ex_data,unsigned char* filepath)
{
    FILE* local_file;
    local_file = fopen(filepath,"rw");
    if(pfile==NULL) exit(0);

    unsigned char* pfile;
    fseek(pfile,0L,SEEK_END);
    size_t filesize = ftell(pfile);
    ex_data = malloc(filesize);
    fread(ex_data,filesize,1,local_file);

    fclose(local_file);
    free(local_file);
}

int STEG_est_max_in_img(unsigned char* image_data,int datasize)
{
    // Base capacity to increment later
    int capacity = 0;

    // 1 - base LSB check
    capacity += datasize/8;

    // more steps to add

    // return
    return capacity;
}

void STEG_write_bit(unsigned char* data,unsigned char byte);
{
    const unsigned char mask = 0x01;
    for(int i=0;i<8;i++)
    {
        data[i] |= (byte>>i & mask);
    }
}

void STEG_process(unsigned char* ex_data,bwimage_t* image)
{
    for(i=0; i<image->height; i++)
    {
      for(j=0; j<image->width; j++)
      {
        image->data[i][j] = ex_data[i+j*image->width];
      }
    }
}