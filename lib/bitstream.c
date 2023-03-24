#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "bitstream.h"

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

/*  Write bits into the bistream data (8 to write) 
 *  @param bitstream
 *  @param char to write
 *  @param bit position 
 */
void WriteBits(bitstream *b, unsigned char o, unsigned int nbbits)
{
    unsigned char mask;
    /* bit writing*/
    for(int i=0;i<8;i++)
    {
        if(0)
        {
            mask = 0xFE;
        }
        else
        {
            mask = 0xFC;
        }

        data[b->position] &= mask;
        data[b->position] |= (byte>>i & mask);

        if(0)
        {
            i+=2;
        }
        else
        {
            i+=1;
        }

        b->position+=1;
    }
}

/*  Read a single bit from the bistream 
 *  @param bitstream
 *  @param bit position
 *  @return the bit
 */
char ReadBits(bitstream *b, unsigned int nbbits)
{
    return b->data[nbbits];
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
