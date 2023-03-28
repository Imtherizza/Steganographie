#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stegano.h"
#include "message.h"
#include "neobitstream.h"
#include "../pngwrap.h"
#include "color.h"

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
    [JC] Je vois pas trop l'utilité ??
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

//[JC] Permet de calculer un puissance entiere sans utiliser la fonction pow
int STEG_power(int a, int b)
{
    if(!b) 
        return 1;
    int ret=1;
    for(int i=0;i<b;i++)
        ret*=a;
    return ret;
}

//[JC]Transforme un message en bitstream
bitstream* STEG_MessageToBitstream(message *m){
    bitstream *b = CreateEmptyBitstream();

    int size=4+1+4+4+m->size_of_data+m->size_of_filename;
    b->data=malloc(size*sizeof(char));
    b->size=size;//[JC] D'apres la structure de message.h => CRC = 4 octets, sof =1 octet etc... //Donc c'est en octet!!

    for(int i=0;i<4;i++){ //Décompositon du CRC en octet
        b->data[i]=(m->crc&(0xFF<<((3-i)*8)))>>((3-i)*8);
    }
    b->data[4]=m->size_of_filename; //Injection de l'octet directement
    for(int i=0;i<4;i++){ //Decomposiotn de SizeOfData
        b->data[i+5]=(m->size_of_data&(0xFF<<((3-i)*8)))>>((3-i)*8);
    }
    
    for(int i=0;i<4;i++){//Decomposition de SizeOfOption
        b->data[i+9]=(m->size_of_option&(0xFF<<((3-i)*8)))>>((3-i)*8);
    }

    for(int i=0;i<m->size_of_filename;i++){ //Decomposition de filename en octet
         b->data[i+13]=m->filename[i];
    }

    for(int i=0;i<m->size_of_data;i++){ //Attention, ici size_of_date contient bien size_of_option car on a deja INJECTE option dans le nouveau data!
        b->data[i+13+m->size_of_filename]=m->data[i];
    }

    return b;
}

//[JC] Rappel, on a un bitstream qui est sale qui contient toute l'image.
message* STEG_BitstreamToMessage(bitstream *b){

    message *m=malloc(sizeof(message));
    m->crc=0;
    m->size_of_filename=0;
    m->size_of_data=0;
    m->size_of_option=0;

    //We extract the message part, we will ignore the theorical junk at the end
    for (int i = 0; i < 4; i++)
    {
        m->crc+=(b->data[i])<<((3-i)*8);
        b->position++;
    }

    m->size_of_filename=b->data[4];

    b->position++;
    
    for (int i = 0; i < 4; i++)
    {
        m->size_of_data+=(b->data[i+5])<<(3-i)*8;

        b->position++;
    }

    for(int i=0;i<4;i++){
        m->size_of_option+=(b->data[i+9])<<(3-i)*8;
        b->position++;
    }

    m->filename=malloc(m->size_of_filename);
    for(int i=0;i<m->size_of_filename;i++){
        m->filename[i]=b->data[b->position+i];
    }
    b->position+=m->size_of_filename;


    char* options=malloc(m->size_of_option); //Options enable us to carry more data, maybe nested steganography :p
    m->data=malloc(m->size_of_data);

    for(int i=0;i<m->size_of_data;i++){
        if(i<m->size_of_option){
            options[i]=b->data[b->position+i];
        }else{
            m->data[i-m->size_of_option]=b->data[i+b->position];
        }

    }
    b->position+=m->size_of_data;
    return m;    
}



void ConvertColorToSTEG_IMG(STEG_IMG *NewIMG,rgba_image_t *image){
    NewIMG->xmax=image->width;
    NewIMG->ymax=image->height;
    NewIMG->RGB=ConvertToRGB(image->data,image->width,image->height,1);
}

void ConvertBWToSTEG_IMG(STEG_IMG *NewIMG,bwimage_t *image){
    STEG_IMG *artifice=malloc(sizeof(STEG_IMG));
    NewIMG->xmax=image->width;
    NewIMG->ymax=image->height;
    NewIMG->RGB=ConvertToRGB(image->data,image->width,image->height,0);
}

void ConvertBackRGBAToPNG(STEG_IMG *EditedIMG,rgba_image_t *image){
    image->data=ReconvertToPNG(EditedIMG->RGB,image->width,image->height,1);
}

void ConvertBackBWToPNG(STEG_IMG *EditedIMG,bwimage_t *image){
    image->data=ReconvertToPNG(EditedIMG->RGB,image->width,image->height,0);
}

//Créer la matrice de position pixel sous forme d'un vecteur
char* CreateOptionData(char* key,int message_size,int imgx,int imgy){
    int seed=0;
    int count=0;
    while (key[count]!='\0'){
        seed+=key[count];
        count++;
    }
    

    srand(seed); //Init le random gen.
    char *ret=malloc(imgx*imgy*sizeof(char)+1);

    for(int i=0;i<imgx;i++){ //On init le tableau
        for(int j=0;j<imgy;j++){
            ret[i+j*imgx]=0;
        }
    }
    int targetNumber=0;
    int xrand=0;
    int yrand=0;
    while(targetNumber<message_size){
        xrand = rand()%imgx;
        yrand = rand()%imgy;

        if(ret[xrand+yrand*imgx]==0){
            ret[xrand+yrand*imgx]=1;
            targetNumber++;
        }
    }
    ret[imgx*imgy]='\0';

    
    return ret;
}