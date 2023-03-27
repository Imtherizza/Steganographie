/*
 * Created on Thu Mar 23 2023
 *
 * Copyright (c) 2008-2023 : Naoki Takei (JC)
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pngwrap.h"
#include "lib/stegano.h"
#include "lib/color.h"


//FONCTIONS
void ConvertColorToSTEG_IMG(STEG_IMG *NewIMG,rgba_image_t *image);
void ConvertBWToSTEG_IMG(STEG_IMG *NewIMG,bwimage_t *image);
void ConvertBackRGBAToPNG(STEG_IMG *EditedIMG,rgba_image_t *image);
void ConvertBackBWToPNG(STEG_IMG *EditedIMG,bwimage_t *image);
char* CreateOptionData(char* key,int message_size,int imgx,int imgy);


int main(int argc,char **argv)
{
    /*
    for(int k=0;k<argc;k++){
        printf("argc[%d] ; argv[%d]=%s \n",k,k,argv[k]);
    }
    printf("End Debug\n\n");*/
    
    char input[32];
    char artifice[70];strcpy(artifice,  "../");
    char artifice2[70];strcpy(artifice2,  ".png");

    char* FileNameStr=NULL; //Nom du fichier 
    char* EncryptionStyle=NULL; //Contient la clef

    int bNoOption=1;
    int bIsColored=0;

    if(argc>1){
        if(argc%2!=0){
                for(int i=0;i<argc/2;i++){ //Nom de la sous commande puis argument de la sous commande
                    if(strlen(argv[i*2+2])>31){
                        printf("Invalid argument!\n");
                        exit(-1);
                    }
                    strcpy(input,argv[i*2+1]);
                    //printf("%d %s \n",i,input);
                    if(!strcmp(input,"-file") || !strcmp(input, "-f") || !strcmp(input,"-F") || !strcmp(input,"-FILE") || !strcmp(input,"-File")){
                        FileNameStr = malloc(strlen(argv[i*2+2])+1);
                        strcpy(FileNameStr,argv[i*2+2]);
                    }
                    if(!strcmp(input,"-Encryption") || !strcmp(input, "-encryption") || !strcmp(input,"-ENCRYPTION") || !strcmp(input,"-E") || !strcmp(input,"-e") || !strcmp(input,"-mode") || !strcmp(input,"-Mode") || !strcmp(input,"-MODE") || !strcmp(input,"-m") || !strcmp(input,"-M")){
                        EncryptionStyle = malloc(strlen(argv[i*2+2])+1);
                        strcpy(EncryptionStyle,argv[i*2+2]);
                    }  
                    if(!strcmp(input,"-Color") || !strcmp(input, "-COLOR") || !strcmp(input,"-color") || !strcmp(input,"-C") || !strcmp(input,"-c")){
                        bIsColored=atoi(argv[i*2+2]);
                    }  
                }
        }else{
            printf("Missing an argument!\n");
            exit(-1);
        }
    }

    if(FileNameStr==NULL){
        printf("Couldn't find input image!\n");
        exit(-1);
    }


    if(EncryptionStyle!=NULL){
        bNoOption=0;
    }else{ //Permet d'éviter quelques problemes en bas...
        EncryptionStyle = malloc(1);
        EncryptionStyle[0]='\0';
    }
    
    strcat(artifice,FileNameStr); //On a : ../nameSTR stocké dans artifice
    strcpy(FileNameStr,artifice); //Permet de remettre __dans le bon ordre__ le str.
    

    if(bIsColored){


        //PARTIE 1: //Read the image -> read every bit -> bitstream -> message || Partie 2:-> Into STEG_IMG -> PNG DUMP.
        
        rgba_image_t *image;//Open the targeted image.
        error_e imageretval=E3A_OK;
        image=E3ACreateRGBAImage();

        //On charge l'image
        if(E3A_OK != (imageretval=E3ALoadRGBAImage(FileNameStr, image))){
            printf("Cannot open the targeted image!\n");
            exit(-1);
        }

        //On converti en STEG_IMG car c'est plus facile a gerer les pixels.
        STEG_IMG *ConvertedIMG=malloc(sizeof(STEG_IMG));
        ConvertColorToSTEG_IMG(ConvertedIMG,image);

        bitstream *bitstreamOutput;
        bitstreamOutput = ReadBitsLin(ConvertedIMG->RGB,ConvertedIMG->xmax,ConvertedIMG->ymax,1);

        message *decryptedMessage;
        decryptedMessage = STEG_BitstreamToMessage(bitstreamOutput);

        DumpImageFromMessage(decryptedMessage);
      


        //Free

        E3AFreeRGBAImage(image);
        free(ConvertedIMG);
        FreeBitstream(bitstreamOutput);
        FreeMessage(decryptedMessage);

        free(FileNameStr);

    }else{
        //PARTIE 1: //Read the image -> read every bit -> bitstream -> message || Partie 2:-> Into STEG_IMG -> PNG DUMP.
        
        bwimage_t *image;//Open the targeted image.
        error_e imageretval=E3A_OK;
        image=E3ACreateImage();

        //On charge l'image
        if(E3A_OK != (imageretval=E3ALoadImage(FileNameStr, image))){
            printf("Cannot open the targeted image!\n");
            exit(-1);
        }

        //On converti en STEG_IMG car c'est plus facile a gerer les pixels.
        STEG_IMG *ConvertedIMG=malloc(sizeof(STEG_IMG));
        ConvertBWToSTEG_IMG(ConvertedIMG,image);

        bitstream *bitstreamOutput;
        bitstreamOutput = ReadBitsLin(ConvertedIMG->RGB,ConvertedIMG->xmax,ConvertedIMG->ymax,0);

        message *decryptedMessage;
        decryptedMessage = STEG_BitstreamToMessage(bitstreamOutput);


        DumpImageFromMessage(decryptedMessage);
      


        //Free

        E3AFreeImage(image);
        free(ConvertedIMG);
        FreeBitstream(bitstreamOutput);
        FreeMessage(decryptedMessage);

        free(FileNameStr);

    }
    return 0;
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
    char *ret=malloc(imgx*imgy*sizeof(char));
    for(int i=0;i<imgx;i++){ //On init le tableau
        for(int j=0;j<imgy;j++){
            ret[i+j*imgx]=0;
        }
    }
    int targetNumber=0;
    int xrand=0;
    int yrand=0;
    while(targetNumber!=message_size){
        xrand = rand()%imgx;
        yrand = rand()%imgy;

        if(ret[xrand+yrand*imgx]==0){
            ret[xrand+yrand*imgx]=1;
            targetNumber++;
        }
    }



    return ret;
}