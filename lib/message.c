#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <png.h>
#include "message.h"
#include "zlib.h"

message* CreateFromFile(const char *filepath)
{
    message* m = malloc(sizeof(message));
    m->size_of_data = 0;
    m->size_of_filename = 0;

    /* Extract file data */ //[JC]Plutot Open the file
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
    m->size_of_data = filesize; //[JC] On get la taille du fichier, pour allouer.
    fread(m->data,sizeof(char),filesize,file);//[JC]On lit pour stocker dans data
    /* CRC */
    ComputeCRC(m);

    /* Filename */
    m->size_of_filename=strlen(filepath);
    m->filename = malloc(m->size_of_filename);
    for(int i=3;i<m->size_of_filename;i++){
        m->filename[i-3]=filepath[i];
    }

    fclose(file);
    return m;
}

unsigned int ComputeCRC(message* m)
{
    unsigned int crc = crc32(0, NULL, 0);
    unsigned int i=0;
    while(i < m->size_of_data) {
        crc = crc32(crc, m->data, m->size_of_data);
        i++;
    }
    m->crc=crc;
}

message* Reallocate(message *m)
{
    return m;
}

void FreeMessage(message *m)
{

    free(m->filename);
    free(m->data);
    free(m);
}


void InjectOptionIntoData(message *m,char* option){
    unsigned int option_size = strlen(option);
    unsigned int data_size = m->size_of_data;
    m->size_of_option = option_size;

    char* newdata = malloc(data_size+option_size);
    printf("%d\n",option_size);
    for(int i=0;i<option_size;i++){ //[JC]On injecte d'abord le data provenant des options
        newdata[i]=option[i];
    }
    for(int i=data_size;i<data_size+option_size;i++){//[JC]Puis on injecte le data de l'image.
        newdata[i]=m->data[i-data_size];
    }

    free(m->data); //On libere le bloc de l'ancienne données
    m->data = malloc(data_size+option_size); //Et on reallou pour le nouveau bloc de data et on attribue les données.
    m->size_of_data=data_size+option_size;
    m->data = newdata;

    free(newdata);
}


void DumpImageFromMessage(message *m){
    FILE *fp;
    
    fp = fopen(m->filename, "wb");
    
    if(fp == NULL)
    {
        printf("Couldnt create file or access file!\n");   
        exit(-1);             
    }
    fwrite(m->data, sizeof(char), m->size_of_data, fp);
    fclose(fp);
}