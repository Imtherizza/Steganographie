#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "lib/bitstream.h"

/*  Creates an empty bitstream with no data, ready to use
 *  @return The empty bistream
 */
bitstream* CreateEmptyBitstream()
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
bitstream* CreateBitstreamOnData(char *data, unsigned int size)
{
    bitstream *ret_bitstream = malloc(sizeof(bitstream));
    ret_bitstream->size = size;
    ret_bitstream->capacity = size/8;
    ret_bitstream->position = 0;
    ret_bitstream->data = malloc(sizeof(short)*size)
    for(int i=0;i<data;i++)
        ret_bitstream->data[i] = data[i];
    
    return ret_bitstream;
}

/*  could use b->data[b.position]
 *  @param the bitstream to reset the position of
 */
void ResetPosition(bitstream *b)
{
    b->position = 0;
}

/* Assuming correct order
 *
 *
 *
 */
void WriteBits(bitstream *b, char o, unsigned int nbbits)
{
    for(int i=b->position;i<nbbits;i++)
    {
        b->data[i]=o;
    }
}

char ReadBits(bitstream *b, unsigned int nbbits)
{
    //????
}

/*  Reset the bitstream 
 *
 *
 *
 * 
 */
void FreeBitstream(bitstream *b)
{
    if (b->data)
        free(b->data);
    free(b);
}

