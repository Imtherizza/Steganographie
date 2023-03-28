#ifndef __NEOBITSTREAM_H__
#define __NEOBITSTREAM_H__

typedef struct
{
    unsigned int size;     // Taille en octets
    unsigned char *data;   // Les donnees
    unsigned int position; // Position dans les données.
} bitstream;

//[JC] Crée un bitstream initialisé a 0
bitstream *CreateEmptyBitstream(void);

//[JC] Crée un bitstream de A à Z a partir d'un data
bitstream *CreateBitstreamOnData(unsigned char *data, unsigned int size);

//[JC]Normalement, quand on utilise cette fonction, on connait deja la taille du data.
// On veut juste écrire dans l'image.
void WriteDataLin(bitstream *b, unsigned char ***image, int xmax, int ymax, int isColored);

void WriteDataRand(bitstream *b, unsigned char ***image, int xmax, int ymax, int isColored, char *Option);

//[JC]Lors du décodage, on peut lire la fichier caché comme un bitstream.
bitstream *ReadBitsLin(unsigned char ***image, unsigned int xmax, unsigned int ymax, unsigned int isColored);
bitstream *ReadBitsRand(unsigned char ***image, unsigned int xmax, unsigned int ymax, unsigned int isColored, char *RandMatrix, int len);

// fait
void FreeBitstream();

#endif //__BITSTREAM_H__
