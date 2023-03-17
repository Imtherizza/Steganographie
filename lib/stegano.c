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
    //printf("%s\n",filepath);
    FILE* local_file;
    unsigned char *hd_data,*f_data;
    size_t hd_size,filesize;
    local_file = fopen(filepath,"rw");
    if(!local_file) 
    {
        exit(-1);
    }
    fseek(local_file,0L,SEEK_END);
    filesize = ftell(local_file);
    fseek(local_file,0L,SEEK_SET);
    f_data = malloc(filesize);
    fread(f_data,sizeof(char),filesize,local_file);
    hd_size = STEG_header_size(filepath);
    hd_data = malloc(hd_size);
    STEG_header_creation(hd_data,filepath,filesize);
    ex_data = malloc(filesize+hd_size);

    for(int x = 0;x<filesize+hd_size;x++)
    {
        if(x<hd_size)
            ex_data[x] = hd_data[x];
        else
            ex_data[x] = f_data[x-hd_size];
    }

    // print debug
    if(0)
    {
        for(int mdr=0;mdr<hd_size+25;mdr++)
        {
            printf("%x\n",ex_data[mdr]);
            if(mdr==hd_size-1) printf("\n");
        }
    }
    //blubblub
    printf("\n%d %d %d\n\n",filesize, hd_size, hd_size+filesize);

    fclose(local_file);
    return filesize+hd_size;
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
        fprintf(local_file,(char)ex_data[i]);
    }
    fclose(local_file);
}

size_t STEG_header_size(unsigned char* filepath)
{
    printf("\n%d\n\n",strlen(filepath));
    int pos = 0;
    for(int i=strlen(filepath);i>=0;i--)
    {
        if(filepath[i]=='/'||!i)
        {
            pos+=strlen(filepath)-i-1;
            break;
        }
    }
    return pos+12;
}

/* in order : CRC32,filname size,file size,filename...
 * cat this with the file to hide such as "strcat()""
 */
size_t STEG_header_creation(unsigned char* ex_data,unsigned char* filepath,size_t filesize)
{
    int pos = 12;
    if(ex_data == NULL)
    {
        exit(-1);
    }
    //printf("szof : %ld\n\n",);
    size_t filename_size;
    unsigned int byte, crc, mask, a=0, j=0;

    // CRC 32
    crc = 0xFFFFFFFF;

    ex_data[0]=(char)((crc >> 24) & 0xFF);
    ex_data[1]=(char)((crc >> 16) & 0xFF);
    ex_data[2]=(char)((crc >> 8) & 0xFF);
    ex_data[3]=(char)((crc >> 0) & 0xFF);

    // filename for decode purposes
    
    for(int i=sizeof(filepath);i>=0;i--)
    {
        if(filepath[i]=='/'||!i)
        {
            filename_size = (strlen(filepath)*sizeof(char))-i-1;
            //printf("%ld\n\n",sizeof(a_data));

            ex_data[4]=(char)(((int)filename_size >> 24) & 0xFF);
            ex_data[5]=(char)(((int)filename_size >> 16) & 0xFF);
            ex_data[6]=(char)(((int)filename_size >> 8) & 0xFF);
            ex_data[7]=(char)(((int)filename_size >> 0) & 0xFF);

            // size_t -> chars
            ex_data[8]=(char)(((int)filesize >> 24)& 0xFF);
            ex_data[9]=(char)(((int)filesize >> 16) & 0xFF);
            ex_data[10]=(char)(((int)filesize >> 8) & 0xFF);
            ex_data[11]=(char)(((int)filesize >> 0) & 0xFF);

            for(int k=0;k<=sizeof(filepath)-i+1;k++)
            {
                //printf("szof : %d\n",strlen(a_data));
                ex_data[12+k] = filepath[i+k+1];
                pos++;
            }
            break;
        }
    }

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

int STEG_decode_data(unsigned char* in_data)
{
    int crc = 0, filename_size = 0,filesize = 0;
    unsigned char* filename;
    // CRC
    crc += in_data[0]<<0;
    crc += in_data[1]<<8;
    crc += in_data[2]<<16;
    crc += in_data[3]<<24;

    // FILENAME SIZE
    filename_size += in_data[4]<<0;
    filename_size += in_data[5]<<8;
    filename_size += in_data[6]<<16;
    filename_size += in_data[7]<<24;

    // FILE SIZE
    filename += in_data[4]<<0;
    filename += in_data[5]<<8;
    filename += in_data[6]<<16;
    filename += in_data[7]<<24;

    // FILENAME
    filename = malloc(filename_size);
    for(int i=0;i<filename_size;i++)
    {
        filename[i] = in_data[8+i];
    }
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
        printf("base data : %x\n",data[i+*position]);
        data[i+*position] &= 0xFE;
        data[i+*position] |= (byte>>i & 0xFE);
        printf("new data : %x\n",data[i+*position]);
    }
    *position+=8;
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