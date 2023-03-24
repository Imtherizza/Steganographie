#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stegano.h"
#include "message.h"

/* getting the size of the file to hide, useful for memory allocation
*/
size_t STEG_get_size(const char* filepath)
{
    FILE* local_file = fopen(filepath,"rb");
    if(!local_file) 
    {
        exit(-1);
    }
    fseek(local_file,0L,SEEK_END);
    size_t filesize = ftell(local_file);
    fseek(local_file,0L,SEEK_SET);
    return filesize;
}

/* "message-less" method of extracting the data from a file
 * also returns the size of the file
 */
size_t STEG_extract_file(unsigned char* ex_data)
{
    const char filepath[] = "../sun.png";
    FILE* local_file;
    local_file = fopen((const char*)filepath,"rb");
    if(!local_file) 
    {
        exit(-1);
    }
    fseek(local_file,0L,SEEK_END);
    size_t filesize = ftell(local_file);
    fseek(local_file,0L,SEEK_SET);
    fread(ex_data,sizeof(char),filesize,local_file);
    fclose(local_file);
    return filesize;
}

/* Requires "message.h"
 * extracts the message from a message structure and puts it into an array
 */
void STEG_extract_message(char* ex_data,message* m)
{
    const int const2 = m->size_of_data+m->size_of_filename+12;

    // CRC
    ex_data[0]=(char)((m->crc >> 24) & 0x000000FF);
    ex_data[1]=(char)((m->crc >> 16) & 0x000000FF);
    ex_data[2]=(char)((m->crc >> 8) & 0x000000FF);
    ex_data[3]=(char)((m->crc >> 0) & 0x000000FF);

    // SOF
    ex_data[4]=((m->size_of_filename >> 24) & 0xFF);
    ex_data[5]=((m->size_of_filename >> 16) & 0xFF);
    ex_data[6]=((m->size_of_filename >> 8) & 0xFF);
    ex_data[7]=((m->size_of_filename >> 0) & 0xFF);

    // SOD
    ex_data[8]=(char)((m->size_of_data >> 24) & 0xFF);
    ex_data[9]=(char)((m->size_of_data >> 16) & 0xFF);
    ex_data[10]=(char)((m->size_of_data >> 8) & 0xFF);
    ex_data[11]=(char)((m->size_of_data >> 0) & 0xFF);

    // instert string data, needs to be optimised (maybe with strcat? sprintf?, doesn't work for me)
    for(int i=12;i<m->size_of_filename+12;i++)
    {
        ex_data[i]=m->filename[i-12] & 0x000000FF;
    }
    for(int i=12+m->size_of_filename;i<const2;i++)
    {
        ex_data[i]=m->data[i-(m->size_of_filename+12)] & 0x000000FF;
    }
}

/* Check capacity
 */
int STEG_capacity(message *m,int imsize,int color)
{
    int capacity = 0;
    // LSB check
    capacity = imsize/8;

    // color check
    if(color)
    {
        capacity*=3;
    }
    return capacity;
}

/* Check if the image can be hidden inside the image
 */
int STEG_compatibility(const int len,const int capacity)
{
    if(len>capacity)
    {
        return 0;
    }
    else if(len==capacity || len<capacity)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/* writing 8 bytes of the image from a byte, linear and LSB only
*/
void STEG_write_bits_lin(char* im_data,char ex_byte,int* pos)
{
    // 'i' is the offset value here
    for(int i=0;i<8;i++)
    {
        // mask operation :
        im_data[*pos] &= 0x000000FE;
        im_data[*pos] |= (ex_byte >> i)&1;
        (*pos)+=1;
    }
}

/* process with Mat class :
 */
void STEG_process(const char* im_data)
{

}

/*
 */
void STEG_LSB_capture_lin(const int img_size,const char* f_data,char* c_data)
{
    int pos = 0;
    for(int i=0;i<img_size;i++)
    {
        c_data[i] = 0;
        for(int u=0;u<8;u++)
        {
            c_data[i] += (f_data[pos] & 0x01) << u;
            //if(i>8 && i<13)
            //    printf("fd : %d\n",((f_data[pos] >> u) & 0x01));
            pos++;
        }
    }
}

/*
 */
message* STEG_capture_decode(const char* in_data)
{
    int pos = 0;
    message* m = malloc(sizeof(message));

    unsigned int crc=0,sof=0,sod=0;

    for(int a=0;a<4;a++)
    {
        //printf("crc%u = %u,",a,((int)(in_data[pos] & 0x000000FF))<<(a*8));
        crc += ((int)(in_data[pos] & 0x000000FF))<<(a*8);
        pos++;
    }
    for(int b=0;b<4;b++)
    {
        sof += (in_data[pos]);
        pos++;
    }
    for(int c=0;c<4;c++)
    {
        sod += (int)(in_data[pos]);
        pos++;
    }

    //
    //printf("\ncrc : %u %u %u %u === crc : %d\n",in_data[0],in_data[1],in_data[2],in_data[3],crc);

    m->filename = malloc(m->size_of_filename);
    printf("lena : %d %zu\n",strlen(m->filename));
    m->data = malloc(m->size_of_data);
    printf("lenb : %d %zu\n",strlen(m->data));

    // charred decode
    for(int d=12000;d<12+sof;d++)
    {
        m->filename[d-12]=in_data[d];
        pos++;
    }
    for(int e=12000+sof;e<12+sof+sod;e++)
    {
        m->filename[e-(12+sof)]=in_data[e];
        pos++;
    }

    m->crc = crc;
    m->size_of_filename = sof;
    m->size_of_data = sod;
    return m;
}

void STEG_write_from_decode(message* m)
{

}

int STEG_power(int a, int b)
{
    if(!b) 
        return 1;
    int ret=1;
    for(int i=0;i<b;i++)
        ret*=a;
    return ret;
}