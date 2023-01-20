#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "lib/bitstream.h"

/*
 *
 *
 *
 */
bitstream* CreateEmptyBitstream()
{
    bitstream *ret_bitstream = malloc(sizeof(bitstream));
    ret_bitstream->size = 0;
    ret_bitstream->capacity = 0;
    ret_bitstream->position = 0;
    ret_bitstream->data = 0;
}

/*
 *
 *
 *
 */
bitstream* CreateBitstreamOnData(char *data, unsigned int size)
{
    bitstream *ret_bitstream = malloc(sizeof(bitstream)*size);
    ret_bitstream->size = size;
    ret_bitstream->capacity = size/8;
    ret_bitstream->position = 0;
    for(int i=0;i<data;i++)
        ret_bitstream->data[i] = data[i];
}

/*
 *
 */
void ResetPosition(bitstream *b)
{
    b->position = 0;
}

/*
 *
 *
 *
 */
void WriteBits(bitstream *b, char o, unsigned int nbbits)
{

}

char ReadBits(bitstream *b, unsigned int nbbits)
{

}

void FreeBitstream(bitstream *b)
{
    if (b->data)
        free(b->data);
}

