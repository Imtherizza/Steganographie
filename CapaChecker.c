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

int main(int argc, char **argv)
{
    char input[32];
    char artifice[70];
    strcpy(artifice, "../");
    char artifice2[70];
    strcpy(artifice2, ".png");

    char *FileNameStr = NULL;     // Nom du fichier
    char *EncryptionStyle = NULL; // Contient la clef

    int bNoOption = 1;
    int FileLength = 0;
    int bIsColored = 0;

    if (argc > 2)
    {
        if (argc % 2 != 0)
        {
            for (int i = 0; i < argc / 2; i++)
            { // Nom de la sous commande puis argument de la sous commande
                if (strlen(argv[i * 2 + 2]) > 31)
                {
                    printf("Invalid argument!\n");
                    exit(-1);
                }
                strcpy(input, argv[i * 2 + 1]);
                if (!strcmp(input, "-file") || !strcmp(input, "-f") || !strcmp(input, "-F") || !strcmp(input, "-FILE") || !strcmp(input, "-File"))
                {
                    FileNameStr = malloc(strlen(argv[i * 2 + 2]) + 1);
                    strcpy(FileNameStr, argv[i * 2 + 2]);
                }
                if (!strcmp(input, "-Color") || !strcmp(input, "-COLOR") || !strcmp(input, "-color") || !strcmp(input, "-C") || !strcmp(input, "-c"))
                {
                    bIsColored = atoi(argv[i * 2 + 2]);
                }
            }
        }
        else
        {
            printf("Missing an argument!\n");
            exit(-1);
        }
    }
    else if (argc == 2)
    {
        strcpy(input, argv[1]);
        if (!strcmp(input, "-Help") || !strcmp(input, "-help") || !strcmp(input, "-HELP") || !strcmp(input, "-H") || !strcmp(input, "-h"))
        {
            printf("Command usage is as follow:\n");
            printf("-f PATHFILE : File name to check the capacity.\n");
            printf("-c 0 or 1 : Are the files in black and white or is it in color ?\n");
            exit(0);
        }
    }

    if (FileNameStr == NULL)
    {
        printf("Couldn't find input image!\n");
        exit(-1);
    }

    strcat(artifice, FileNameStr); // On a : ../nameSTR stocké dans artifice
    strcpy(FileNameStr, artifice); // Permet de remettre __dans le bon ordre__ le str.
    unsigned int capacity=0;
    if (bIsColored)
    {
        rgba_image_t *image; // Open the targeted image.
        error_e imageretval = E3A_OK;
        image = E3ACreateRGBAImage();

        // On charge l'image
        if (E3A_OK != (imageretval = E3ALoadRGBAImage(FileNameStr, image)))
        {
            printf("Cannot open the targeted image!\n");
            exit(-1);
        }
        printf("Image capacity in color is %d.\n",image->width*image->height*3);       

        // Free

        E3AFreeRGBAImage(image);
    }
    else
    {
        bwimage_t *image; // Open the targeted image.
        error_e imageretval = E3A_OK;
        image = E3ACreateImage();
        // On charge l'image
        if (E3A_OK != (imageretval = E3ALoadImage(FileNameStr, image)))
        {
            printf("Cannot open the targeted image!\n");
            exit(-1);
        }
        // Free
        printf("Image capacity in BW is %d.\n",image->width*image->height);  

        E3AFreeImage(image);
    }
    printf("Operation was a success!\n");
    return 0;
}