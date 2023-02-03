#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "lib/message.h"
#include "bitstream.h"
#include "lib/stegano.h"

/*
bitstream* message_to_new_bitstream(message* m)
{
    unsigned int t_m_size = sizeof(int)*sizeof(m->size_of_data)+sizeof(char)*sizeof(m->size_of_filename);
    unsigned char *t_data = malloc(t_m_size);

    //data writing
    unsigned int i=0;
    while(i<)

    bitstream* t_b = CreateBitstreamOnData(unsigned char *t_data, unsigned int size);
}
*/

int CRC_calc(char *bits,int length, int poly)
{
    int shiftRegister = 0xFFFFFFFF;  // Généralement tous les bits à 0 ou 1
    for(int i=0;i<length;i++)
    {
        if( MSB(shiftRegister)^bits[i]==1){
            shiftRegister=(shiftRegister*2)^poly;
        }else{
            shiftRegister=(shiftRegister*2);
        }
    }
    return shiftRegister;
}

int MSB(int n)
{
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n = ((n + 1) >> 1)|(n & (1 << ((sizeof(n) * CHAR_BIT)-1)));
    return n;
}
