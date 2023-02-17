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
    STEG_header_creation(local_file,filepath,filesize);
    fread(ex_data,filesize,1,local_file);
    fclose(local_file);
    return filesize;
}

void STEG_recreate_file(unsigned char* ex_data,unsigned char* filename,size_t filesize)
{
    FILE* local_file;
    local_file = fopen(strcat("output/",filename),"rw");
    if(!local_file) 
    {
        exit(-1);
    }
    for(int i=0;i<filesize;i++)
    {
        fprintf(local_file,ex_data[i]);
    }
    fclose(local_file);
}

/* in order : CRC32,filname size,file size,filename...
 * cat this with the file to hide such as "strcat()""
 */
void STEG_header_creation(unsigned char* ex_data,unsigned char* filepath,size_t filesize)
{
    unsigned char* a_data = malloc(12*sizeof(char));
    size_t filename_size;
    unsigned int byte, crc, mask, a=0, j=0;

    // CRC 32
    crc = 0xFFFFFFFF;
    while (ex_data[a] != 0) 
    {
        byte = ex_data[a]; // Get next byte.
        crc = crc ^ byte;
        for (int b = 7; b >= 0; j--) 
        {   // Do eight times.
            mask = -(crc & 1);
            crc = (crc >> 1) ^ (0xEDB88320 & mask);
        }
        a+=1;
    }
    a_data[0]=(char)(crc & 0xFF);
    a_data[1]=(char)((crc >> 8) & 0xFF);
    a_data[2]=(char)((crc >> 16) & 0xFF);
    a_data[3]=(char)((crc >> 24) & 0xFF);

    // filename for decode purposes
    // to replace with strrchr() in the future
    for(int i=strlen(filepath);i>=0;i--)
    {
        if(filepath[i]=='/'||!i)
        {
            filename_size = strlen(filepath-i)*sizeof(char);
            a_data = realloc(a_data,sizeof(a_data)+8*sizeof(char));
            a_data[j+1]=(char)((int)filename_size & 0xFF);
            a_data[j+2]=(char)(((int)filename_size >> 8) & 0xFF);
            a_data[j+3]=(char)(((int)filename_size >> 16) & 0xFF);
            a_data[j+4]=(char)(((int)filename_size >> 24) & 0xFF);

            // size_t -> chars
            a_data[j+5]=(char)((int)filesize & 0xFF);
            a_data[j+6]=(char)(((int)filesize >> 8) & 0xFF);
            a_data[j+7]=(char)(((int)filesize >> 16) & 0xFF);
            a_data[j+8]=(char)(((int)filesize >> 24) & 0xFF);

            for(j+=8;j<strlen(filepath)-i;j++)
            {
                a_data = realloc(a_data,sizeof(a_data)+sizeof(char));
                a_data[j] = filepath[i+j+1];
            }
        }
    }

    // print debug
    if(1)
    {
        printf("%ld\n",filesize);
        printf("%s\n",a_data);
    }
    
    realloc(a_data,filesize+sizeof(a_data));
    strcat(a_data,ex_data);
    realloc(ex_data,sizeof(a_data));
    ex_data = a_data;
    free(a_data);
}
/*  Linear mode only
 *
 */
unsigned char* STEG_extract_data_LSB_BW(bwimage_t* image,unsigned char* bit_data)
{
    unsigned int pos = 0;
    unsigned char* in_data;

    // extracting the LSB of every pixel
    //image_data = malloc((size_t)image->width*image->height);
    //image_data = image->rawdata;
    for(int i=0;i<(image->width*image->height);i++)
    {
        
    }
}

int STEG_decode_data()
{
    // CRC
    // NAME OF FILE
    //
}

/* Put 0 or 1 for color
 */
unsigned int STEG_est_max_in_img(unsigned char* image_data,int datasize,int color)
{
    // Base capacity to increment later
    int capacity = 0;

    // 1 - base LSB check
    capacity += datasize/8;

    // more steps to add

    // 2 - extreme color check

    // 3 - heavy contrast check

    // 4 - Color check
    if(color==1)
    {
        capacity *= 3;
    }

    // return
    return capacity;
}

/*  Checking the extremes
 */
unsigned int* color_capacity(unsigned char* data)
{

}

unsigned int* contrast_capacity(unsigned char* data)
{

}

/*  WIP writer for all bits of the hidden message, linear
 */
void STEG_write_bits(unsigned char* data,unsigned char* bytes,unsigned int* s_capacity,unsigned int image_size,unsigned int capacity)
{
    int i=0,j,pos,ret_offset;
    unsigned char ret_bit = 0x00;

    while(i<strlen(bytes))
    {
        for(j=0;j<8;j++)
        {
            if(ret_bit!=0)
            {
                ret_offset = 1;
                data[j+pos] &= 0xFE;
                data[j+pos] |= ret_bit;
                // reset the ret_bit 
                ret_bit = 0x00;
            }
            else
            {
                if(s_capacity[j])
                {
                    if(j<7)
                    {
                        data[j+pos] &= 0xFC;
                        data[j+pos] |= (bytes[i]>>j-ret_offset & 0xFC);
                        j++;
                    }
                    else if (j==7)
                    {
                        data[j+pos] &= 0xFE;
                        data[j+pos] |= (bytes[i]>>j-ret_offset & 0xFE);
                        j++;

                        ret_bit = bytes[i]>>j & 0xFE;
                    }
                }
                else
                {
                    data[j+pos] &= 0xFE;
                    data[j+pos] |= (bytes[i]>>j-ret_offset & 0xFE);
                }
            }
            if(j>=7)
            {
                ret_offset = 0x00;
            }
        }
        
        pos+=8;
        i++;

        if(ret_bit && i>=strlen(bytes))
        {
            data[pos+1] &= 0xFE;
            data[pos+1] |= ret_bit;
        }
    }
}

/*  Working writer for basic LSB hiding, needs to be included in a "for" loop
 */
void STEG_write_bit_LSB(unsigned char* data,unsigned char byte,unsigned int* position)
{
    for(int i=0;i<8;i++)
    {
        data[i+(int)position] &= 0xFE;
        data[i+(int)position] |= (byte>>i & 0xFE);
    }
    position+=8;
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

/*  Untested
 */
void STEG_process_RGB_linear(unsigned char* ex_data,rgba_image_t* image)
{
    int k=0;
    for(int j=0; j<image->height; j++)
    {
        for(int i=0; i<image->width; i++)
        {
            image->data[i][4*(j+0)] = ex_data[i+j*image->width+k];
            k++;
            image->data[i][4*(j+1)] = ex_data[i+j*image->width+k];
            k++;
            image->data[i][4*(j+2)] = ex_data[i+j*image->width+k];
            k++;
        }
    }
}