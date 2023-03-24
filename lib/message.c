#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <png.h>
#include "message.h"

message* CreateFromFile(const char *filepath)
{
    message* m = malloc(sizeof(message));
    m->size_of_data = 0;
    m->size_of_filename = 0;

    /* Extract file data */
    FILE* file = fopen(filepath,"rb");
    if(!file) 
    {
        exit(-1);
    }
    fseek(file,0L,SEEK_END);
    size_t filesize = ftell(file);
    //fseek(file,0L,SEEK_SET);
    rewind(file);
    m->data = malloc(filesize);
    m->size_of_data = filesize;
    fread(m->data,sizeof(char),filesize,file);
    /* CRC */
    ComputeCRC(m);

    /* Filename */
    int i;
    for(i=strlen(filepath);filepath[i]!='/';i--);
    m->size_of_filename = strlen(filepath)-i-1;
    m->filename = malloc(m->size_of_filename);
    char* strPath = malloc(strlen(filepath));
    strcpy(strPath,filepath);
    char* strToken = strtok(strPath,"/");
    while(strToken != 0) 
    {
        if(strlen(strToken)==7)
        {
            m->filename = strToken;
        }
        strToken = strtok(0,"/");
    }
    //printf("crc : %u\nsofn : %d\nsofi : %d\nfn : %s\n",m->crc, m->size_of_filename, m->size_of_data, m->filename);
    fclose(file);
    free(strToken);
    free(strPath);

    return m;
}

unsigned int ComputeCRC(message* m)
{
    // placeholder 
    m->crc = 0xFFFFFFFF;
    return 1;
}

message* Reallocate(message *m)
{
    return m;
}

void FreeMessage(message *m)
{

    //free(m->filename);
    //free(m->data);
    free(m);
}