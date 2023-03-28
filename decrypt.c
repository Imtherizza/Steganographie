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

    if (argc > 1)
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
                // printf("%d %s \n",i,input);
                if (!strcmp(input, "-file") || !strcmp(input, "-f") || !strcmp(input, "-F") || !strcmp(input, "-FILE") || !strcmp(input, "-File"))
                {
                    FileNameStr = malloc(strlen(argv[i * 2 + 2]) + 1);
                    strcpy(FileNameStr, argv[i * 2 + 2]);
                }
                if (!strcmp(input, "-Encryption") || !strcmp(input, "-encryption") || !strcmp(input, "-ENCRYPTION") || !strcmp(input, "-E") || !strcmp(input, "-e") || !strcmp(input, "-mode") || !strcmp(input, "-Mode") || !strcmp(input, "-MODE") || !strcmp(input, "-m") || !strcmp(input, "-M"))
                {
                    EncryptionStyle = malloc(strlen(argv[i * 2 + 2]) + 1);
                    strcpy(EncryptionStyle, argv[i * 2 + 2]);
                }
                if (!strcmp(input, "-Length") || !strcmp(input, "-length") || !strcmp(input, "-LENGTH") || !strcmp(input, "-L") || !strcmp(input, "-l"))
                {
                    FileLength = atoi(argv[i * 2 + 2]);
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

    if (FileNameStr == NULL)
    {
        printf("Couldn't find input image!\n");
        exit(-1);
    }

    if (EncryptionStyle != NULL)
    {
        bNoOption = 0;
        if (FileLength == 0)
        {
            printf("Length cannot be equal to 0!\nYou need to put a integer greater than 0 or you forgot to put the -l variable!\nAborting...\n");
            exit(-2);
        }
    }
    else
    { // Permet d'éviter quelques problemes en bas...
        EncryptionStyle = malloc(1);
        EncryptionStyle[0] = '\0';
    }

    strcat(artifice, FileNameStr); // On a : ../nameSTR stocké dans artifice
    strcpy(FileNameStr, artifice); // Permet de remettre __dans le bon ordre__ le str.

    if (bIsColored)
    {

        // PARTIE 1: //Read the image -> read every bit -> bitstream -> message || Partie 2:-> Into STEG_IMG -> PNG DUMP.

        rgba_image_t *image; // Open the targeted image.
        error_e imageretval = E3A_OK;
        image = E3ACreateRGBAImage();

        // On charge l'image
        if (E3A_OK != (imageretval = E3ALoadRGBAImage(FileNameStr, image)))
        {
            printf("Cannot open the targeted image!\n");
            exit(-1);
        }

        // On converti en STEG_IMG car c'est plus facile a gerer les pixels.
        STEG_IMG *ConvertedIMG = malloc(sizeof(STEG_IMG));
        ConvertColorToSTEG_IMG(ConvertedIMG, image);

        bitstream *bitstreamOutput;
        if (bNoOption)
        {
            bitstreamOutput = ReadBitsLin(ConvertedIMG->RGB, ConvertedIMG->xmax, ConvertedIMG->ymax, 1);
        }
        else
        {
            char *RandMatrix = CreateOptionData(EncryptionStyle, FileLength, image->width, image->height);
            bitstreamOutput = ReadBitsRand(ConvertedIMG->RGB, ConvertedIMG->xmax, ConvertedIMG->ymax, 1, RandMatrix, FileLength);
        }

        message *decryptedMessage;
        decryptedMessage = STEG_BitstreamToMessage(bitstreamOutput);

        DumpImageFromMessage(decryptedMessage);

        // Free

        E3AFreeRGBAImage(image);
        free(ConvertedIMG);
        FreeBitstream(bitstreamOutput);
        FreeMessage(decryptedMessage);

        free(FileNameStr);
    }
    else
    {
        // PARTIE 1: //Read the image -> read every bit -> bitstream -> message || Partie 2:-> Into STEG_IMG -> PNG DUMP.

        bwimage_t *image; // Open the targeted image.
        error_e imageretval = E3A_OK;
        image = E3ACreateImage();
        // On charge l'image
        if (E3A_OK != (imageretval = E3ALoadImage(FileNameStr, image)))
        {
            printf("Cannot open the targeted image!\n");
            exit(-1);
        }

        // On converti en STEG_IMG car c'est plus facile a gerer les pixels.
        STEG_IMG *ConvertedIMG = malloc(sizeof(STEG_IMG));
        ConvertBWToSTEG_IMG(ConvertedIMG, image);
        bitstream *bitstreamOutput;

        if (bNoOption)
        {
            bitstreamOutput = ReadBitsLin(ConvertedIMG->RGB, ConvertedIMG->xmax, ConvertedIMG->ymax, 0);
        }
        else
        {
            char *RandMatrix = CreateOptionData(EncryptionStyle, FileLength, image->width, image->height);
            bitstreamOutput = ReadBitsRand(ConvertedIMG->RGB, ConvertedIMG->xmax, ConvertedIMG->ymax, 0, RandMatrix, FileLength);
        }

        message *decryptedMessage;
        decryptedMessage = STEG_BitstreamToMessage(bitstreamOutput);

        DumpImageFromMessage(decryptedMessage);

        // Free

        E3AFreeImage(image);
        free(ConvertedIMG);
        FreeBitstream(bitstreamOutput);
        FreeMessage(decryptedMessage);

        free(FileNameStr);
    }
    printf("Operation was a success!\n");
    return 0;
}