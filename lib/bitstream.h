#ifndef __BITSTREAM_H__
#define __BITSTREAM_H__

/*  Bitstream
 *
 *
 */
typedef struct
{
    unsigned int size; // Taille en octets
    unsigned int capacity;
    unsigned int position; // Position en bits
    unsigned short *data; // Les donnees
} 
bitstream;

/*
bitstream* CreateEmptyBitstream();

bitstream* CreateBitstreamOnData(char *data, unsigned int size);

void ResetPosition(bitstream *b);

void WriteBits(bitstream *b, char o, unsigned int nbbits);

char ReadBits(bitstream *b, unsigned int nbbits);

void FreeBitstream();
*/

#endif//__BITSTREAM_H__
