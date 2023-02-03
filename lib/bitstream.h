#ifndef __BITSTREAM_H__
#define __BITSTREAM_H__

//#include "../lib/bitstream.c"

/*  Bitstream
 *
 *
 */
typedef struct
{
    unsigned int size; // Taille en octets
    unsigned int capacity;
    unsigned int position; // Position en bits
    unsigned char *data; // Les donnees
} 
bitstream;

//fait
bitstream* CreateEmptyBitstream(void);

//presque fait
bitstream* CreateBitstreamOnData(unsigned char *data, unsigned int size);

//fait
void ResetPosition(bitstream *b);

//non
void WriteBits(bitstream *b, unsigned char o, unsigned int nbbits);

//non
void WriteBit(bitstream *b, unsigned char bit);

//non
char ReadBits(bitstream *b, unsigned int nbbits);

//fait
void FreeBitstream();


#endif//__BITSTREAM_H__
