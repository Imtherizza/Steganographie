#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <string.h>
//#include "lib/message.h"
//#include "lib/bitstream.h"
#include "../pngwrap.h"
#include "stegano.h"

size_t STEG_extract_file(unsigned char* ex_data,unsigned char* filepath)
{
    printf("%s\n",filepath);
    FILE* local_file;
    local_file = fopen(filepath,"rw");
    if(!local_file) 
    {
        exit(-1);
    }
    fseek(local_file,0L,SEEK_END);
    size_t filesize = ftell(local_file);
    ex_data = malloc(filesize);
    STEG_header_creation(local_file,filepath,&filesize);
    fread(ex_data,filesize,1,local_file);
    fclose(local_file);
    return filesize;
}

unsigned char* STEG_header_creation(FILE* file,unsigned char* filepath,size_t* filesize)
{
    unsigned char* first_data = malloc(strlen(filepath)+sizeof(filesize)*sizeof(size_t));
    int j;

    // filename for decode purposes
    for(int i=strlen(filepath);i>0;i--)
    {
        if(filepath[i]=='/')
        {
            for(j=0;j<strlen(filepath)-i;j++)
            {
                first_data[j] = filepath[i+j+1];
                filesize++;
            }
        }
    }

    // size_t -> chars
    first_data[j+1]=(char)((int)filesize & 0xFF);
    first_data[j+2]=(char)(((int)filesize >> 8) & 0xFF);
    first_data[j+3]=(char)(((int)filesize >> 16) & 0xFF);
    first_data[j+4]=(char)(((int)filesize >> 24) & 0xFF);

    printf("%ld\n",filesize);
    printf("%x\n",first_data[j+2]);

    // CRC
    if(0)
    {

    }
}

unsigned int STEG_est_max_in_img(unsigned char* image_data,int datasize,int color)
{
    // Base capacity to increment later
    int capacity = 0;

    // 1 - base LSB check
    capacity += datasize/8;

    // more steps to add

    // 5 - Color check
    if(color)
    {
        capacity*=3;
    }

    // return
    return capacity;
}

void STEG_write_bit(unsigned char* data,unsigned char byte,unsigned int position)
{
    for(int i=position;i<8+position;i++)
    {
        data[i] |= (byte>>i & 0x01);
    }
}

void STEG_process_GRAY_linear(unsigned char* ex_data,bwimage_t* image)
{
    for(int i=0; i<image->height; i++)
    {
        for(int j=0; j<image->width; j++)
        {
            image->data[j][i] = ex_data[i+j*image->width];
        }
    }
}

void STEG_process_RGB_linear(unsigned char* ex_data,rgba_image_t* image)
{
    for(int j=0; j<image->height; j++)
    {
        for(int i=0; i<image->width; i++)
        {
            image->data[i][4*(j+0)] = ex_data[i+j*image->width];
            image->data[i][4*(j+1)] = ex_data[i+j*image->width];
            image->data[i][4*(j+2)] = ex_data[i+j*image->width];
        }
    }
}