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


int main(int argc,char **argv)
{   
    char input[32];
    char artifice[70];strcpy(artifice,  "../");
    char artifice2[70];strcpy(artifice2,  "../");

    char* FileNameStr=NULL;
    char* HiddenNameStr=NULL;
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
                    if(!strcmp(input,"-file") || !strcmp(input, "-f") || !strcmp(input,"-F") || !strcmp(input,"-FILE") || !strcmp(input,"-File")){
                        FileNameStr = malloc(strlen(argv[i*2+2])+1);
                        strcpy(FileNameStr,argv[i*2+2]);
                    }
                    if(!strcmp(input,"-hide") || !strcmp(input, "-Hide") || !strcmp(input,"-HIDE") || !strcmp(input,"-h")|| !strcmp(input,"-H")){
                        HiddenNameStr = malloc(strlen(argv[i*2+2])+1);
                        strcpy(HiddenNameStr,argv[i*2+2]);
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
    if(HiddenNameStr==NULL){
        printf("Couldn't find image to hide!\n");
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

    strcat(artifice2,HiddenNameStr);
    strcpy(HiddenNameStr,artifice2); //Permet de remettre __dans le bon ordre__ le str.

    

    if(bIsColored){        

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
        
        //On crée le message a partir du fichier qu'on veut cacher
        message* HiddenFile = CreateFromFile(HiddenNameStr);

        char* optionData = CreateOptionData(EncryptionStyle,(4+1+4+4+HiddenFile->size_of_filename+HiddenFile->size_of_data)*8,image->width,image->height);

        if(bNoOption==0){
            //On doit donc créer une matrice de booléan(de la taille d'une image) contenant HiddenFile->size fois UN.
            //InjectOptionIntoData(HiddenFile,optionData);
            //HiddenFile->size_of_option=strlen(optionData);
            printf("You used random data injection!\nHere's the data length:%d\n",(4+1+4+4+HiddenFile->size_of_filename+HiddenFile->size_of_data)*8);
            HiddenFile->size_of_option=0;
        }else{
            HiddenFile->size_of_option=0;
        }

        bitstream* message = STEG_MessageToBitstream(HiddenFile);

        if(message->size*8 > ConvertedIMG->xmax*ConvertedIMG->ymax*3){
            printf("Image to hide is too big for the targeted file!\n");
            exit(-1);
        }

        if(bNoOption){
            WriteDataLin(message,ConvertedIMG->RGB,ConvertedIMG->xmax,ConvertedIMG->ymax,1);
        }else{//With rand
            WriteDataRand(message,ConvertedIMG->RGB,ConvertedIMG->xmax,ConvertedIMG->ymax,1,optionData);
        }


        //PARTIE 2: STEG_IMG -> PNG -> DUMP
        ConvertBackRGBAToPNG(ConvertedIMG,image);

        if(E3A_OK != (imageretval=E3ADumpRGBAImage("Encrypted.png", image))){
            printf("Something went wrong!\n");
            exit(-1);
        }


        //Free
        E3AFreeRGBAImage(image);
        free(ConvertedIMG);
        free(HiddenFile);
        free(message);

        free(FileNameStr);
        free(HiddenNameStr);


    }else{
        //PARTIE 1: //Read the image to be written -> transform into "message" -> bitstream -> Insertion.
        
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

        //On crée le message a partir du fichier qu'on veut cacher
        message* HiddenFile = CreateFromFile(HiddenNameStr);



        char* optionData = CreateOptionData(EncryptionStyle,(4+1+4+4+HiddenFile->size_of_filename+HiddenFile->size_of_data)*8,image->width,image->height);

        if(bNoOption==0){
            //On doit donc créer une matrice de booléan(de la taille d'une image) contenant HiddenFile->size fois UN.
            //InjectOptionIntoData(HiddenFile,optionData);
            //HiddenFile->size_of_option=strlen(optionData);
            
            printf("You used random data injection!\nHere's the data length:%d\n",(4+1+4+4+HiddenFile->size_of_filename+HiddenFile->size_of_data)*8);
            HiddenFile->size_of_option=0;
        }else{
            HiddenFile->size_of_option=0;
        }

        bitstream* message = STEG_MessageToBitstream(HiddenFile);
        if(message->size*8 > ConvertedIMG->xmax*ConvertedIMG->ymax){
            printf("Image to hide is too big for the targeted file!\n");
            exit(-1);
        }

        if(bNoOption){
            WriteDataLin(message,ConvertedIMG->RGB,ConvertedIMG->xmax,ConvertedIMG->ymax,0);
        }else{//With rand
            WriteDataRand(message,ConvertedIMG->RGB,ConvertedIMG->xmax,ConvertedIMG->ymax,0,optionData);
        }
        //PARTIE 2: STEG_IMG -> PNG -> DUMP
        ConvertBackBWToPNG(ConvertedIMG,image);
        if(E3A_OK != (imageretval=E3ADumpImage("Encrypted.png", image))){
            printf("Something went wrong!\n");
            exit(-1);
        }


        //Free

        E3AFreeImage(image);
        free(ConvertedIMG);
        free(HiddenFile);
        free(message);

        free(FileNameStr);
        free(HiddenNameStr);

    }
    return 0;
}