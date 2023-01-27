#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "lib/bitstream.h"

/*  Creates an empty bitstream with no data, ready to use
 *  @return The empty bistream
 */
bitstream* CreateEmptyBitstream(void)
{
    bitstream *ret_bitstream = malloc(sizeof(bitstream));
    ret_bitstream->size = 0;
    ret_bitstream->capacity = 0;
    ret_bitstream->position = 0;
    ret_bitstream->data = 0;
    return ret_bitstream;
}

/*  Creates a bitstream with data
 *  @param array of data to put iunside the bitstream
 *  @param size of data array
 *  @return the bitstream with data inside
 */
bitstream* CreateBitstreamOnData(unsigned char *data, unsigned int size)
{
    bitstream *ret_bitstream = malloc(sizeof(bitstream));
    ret_bitstream->size = size;
    ret_bitstream->capacity = size/8;
    ret_bitstream->position = 0;
    ret_bitstream->data = malloc(sizeof(char)*size);
    ret_bitstream->data=data;

    return ret_bitstream;
}

/*  Resets position of bitstream header
 *  @param the bitstream to reset the position of
 */
void ResetPosition(bitstream *b)
{
    b->position = 0;
}

/*  Write bits into the bistream 
 *  @param bitstream
 *  @param char to write
 *  @param bit position 
 */
void WriteBits(bitstream *b, unsigned char o, unsigned int nbbits)
{
    for(int i=b->position;i<nbbits;i++)
    {
        b->data[i]=o;
    }
}
/*
void WriteBit(bitstream *b, unsigned char bit)
{
    size_t bit_pos,cbt = b->position/8;
    if(cbt>=b->size)
    {
        if(b->size>=b->capacity)
        {
           realloc(b->capacity,2*sizeof(b->capacity));
        }
        b->size+=1;
    }
    //masks
    unsigned char mask1 = 0xFF |= (1<<7-bit_pos);
    unsigned char mask2 = (bit_pos + 1)<<1-bit_pos;
    b->posision+=1;
}*/

/*  Read a single bit from the bistream 
 *  @param bitstream
 *  @param bit position
 *  @return the bit
 */
char ReadBits(bitstream *b, unsigned int nbbits)
{
    return b->data[b->position+nbbits];
}

/*  Reset the bitstream 
 *  @param bitstream to free
 */
void FreeBitstream(bitstream *b)
{
    if (b->data)
        free(b->data);
    free(b);
}
