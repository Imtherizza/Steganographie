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

bitstream* CreateEmptyBitstream(void);

bitstream* CreateBitstreamOnData(unsigned char *data, unsigned int size);

void ResetPosition(bitstream *b);

void WriteBits(bitstream *b, unsigned char o, unsigned int nbbits);

//void WriteBit(bitstream *b, unsigned char bit);

char ReadBits(bitstream *b, unsigned int nbbits);

void FreeBitstream();


#endif//__BITSTREAM_H__
