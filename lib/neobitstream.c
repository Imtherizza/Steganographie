#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "neobitstream.h"

/*  Creates an empty bitstream with no data, ready to use
 *  @return The empty bistream
 */
bitstream* CreateEmptyBitstream(void)
{
    bitstream *ret_bitstream = malloc(sizeof(bitstream));
    ret_bitstream->size = 0;
    ret_bitstream->data = 0;
    ret_bitstream->position = 0;
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
    ret_bitstream->data = malloc(sizeof(char)*size);
    ret_bitstream->data=data;
    ret_bitstream->position = 0;

    return ret_bitstream;
}


void WriteDataLin(bitstream *b, unsigned char ***image,int xmax,int ymax, int isColored){
    int bitpos=0;
    if(isColored){
        for(int i=0;i<xmax;i++){
            for(int j=0;j<ymax;j++){ //Pour chaque pixels de l'image, donc 1 octet
                for(int k=0;k<3;k++){ //pas 4 car on veut pas ecrire sur le canal de translucense(alpha channel)
                    unsigned char bit = ((b->data[b->position])&(0x01<<(7-bitpos)))>>(7-bitpos);
                    unsigned char otherbits = image[j][i][k]&0xFE;
                    image[j][i][k]=otherbits+bit;
                    bitpos++;
                    if(bitpos>=8){ //Tant qu'on a pas écrit l'octet en entier, on ne bouge pas!
                        bitpos=0;
                        b->position++;
                        if(b->position>=b->size){
                            return;
                        }
                    }
                }
            }
        }
    }else{
        for(int i=0;i<xmax;i++){
            for(int j=0;j<ymax;j++){ //Pour chaque pixels de l'image, donc 1 octet
                unsigned char bit = ((b->data[b->position])&(0x01<<(7-bitpos)))>>(7-bitpos);
                unsigned char otherbits = image[j][i][0]&0xFE;
                image[j][i][0]=otherbits+bit;
                bitpos++;
                if(bitpos>=8){ //Tant qu'on a pas écrit l'octet en entier, on ne bouge pas!
                    bitpos=0;
                    b->position++;
                    if(b->position>=b->size){
                        return;
                    }
                }
            }
        }
    }
}

void WriteDataRand(bitstream *b, unsigned char ***image,int xmax,int ymax, int isColored, char *Option){
    int bitpos=0;
    if(isColored){
        for(int i=0;i<xmax;i++){
            for(int j=0;j<ymax;j++){ //Pour chaque pixels de l'image, donc 1 octet
                if(Option[i+j*xmax]==1){
                    for(int k=0;k<3;k++){ //pas 4 car on veut pas ecrire sur le canal de translucense(alpha channel)
                        unsigned char bit = (b->data[b->position]&(0x01<<(7-bitpos)))>>(7-bitpos);
                        unsigned char otherbits = image[j][i][k]&0xFE;
                        image[j][i][k]=otherbits+bit;

                        bitpos++;
                        if(bitpos>=8){ //Tant qu'on a pas écrit l'octet en entier, on ne bouge pas!
                            bitpos=0;
                            b->position++;
                            if(b->position>=b->size){
                                return;
                            }
                        }
                    }
                }
            }
        }
    }else{
        for(int i=0;i<xmax;i++){
            for(int j=0;j<ymax;j++){ //Pour chaque pixels de l'image, donc 1 octet
                if(Option[i+j*xmax]==1){
                    unsigned char bit = ((b->data[b->position])&(0x01<<(7-bitpos)))>>(7-bitpos);
                    unsigned char otherbits = image[j][i][0]&0xFE;
                    image[j][i][0]=otherbits+bit;
                    bitpos++;
                    if(bitpos>=8){ //Tant qu'on a pas écrit l'octet en entier, on ne bouge pas!
                        bitpos=0;

                        b->position++;
                        if(b->position>=b->size){
                            return;
                        }
                    }
                }
            }
        }
    }
}

/*  Read a bits from an array 3D
 *  @param bitstream
 *  @param data
 *  @param image x_max
 *  @param image y_max
 */
bitstream* ReadBitsLin(unsigned char*** image, unsigned int xmax, unsigned int ymax, unsigned int isColored){
    bitstream *btemp=CreateEmptyBitstream();
    int bitpos=0;
    int count=0;


    bitstream *btemp2=CreateEmptyBitstream();//The aim is to compact to each 8 bits into 1 byte
    if(isColored){
        btemp->size=xmax*ymax*3;
        btemp->data=malloc(xmax*ymax*3);//Théoriquement, on pourrait avoir un bitstream sur toute la plage de donné de l'image.

        for(int i=0;i<xmax;i++){
            for(int j=0;j<ymax;j++){
                for(int k=0;k<3;k++){
                    btemp->data[k+j*3+i*3*ymax]=image[j][i][k]&0x01;//We read all the channels except alpha.
                }
            }
        }
        
        btemp2->size=xmax*ymax*3/8;
        btemp2->data=malloc(xmax*ymax*3/8);
        while(btemp->position!=btemp->size){
            btemp2->data[count]+=((btemp->data[btemp->position])<<(7-bitpos));

            btemp->position++;
            bitpos++;
            if(bitpos>=8){
                bitpos=0;
                count++;
            }
        }
    }else{
        btemp->size=xmax*ymax;
        btemp->data=malloc(xmax*ymax);//Théoriquement, on pourrait avoir un bitstream sur toute la plage de donné de l'image.

        for(int i=0;i<xmax;i++){
            for(int j=0;j<ymax;j++){
                btemp->data[j+i*ymax]=image[j][i][0]&0x01;//We use the red channel
            }
        }
        btemp2->size=xmax*ymax/8;
        btemp2->data=malloc(xmax*ymax/8);
        while(btemp->position!=btemp->size){
            btemp2->data[count]+=((btemp->data[btemp->position])<<(7-bitpos));
            btemp->position++;
            bitpos++;
            if(bitpos>=8){
                bitpos=0;
                count++;
            }
        }
    }
    FreeBitstream(btemp);

    return btemp2;
}



bitstream* ReadBitsRand(unsigned char*** image, unsigned int xmax, unsigned int ymax, unsigned int isColored, char* Matrix,int len){
    bitstream *btemp=CreateEmptyBitstream();
    int bitpos=0;
    int randptr=0;
    int count=0;

    bitstream *btemp2=CreateEmptyBitstream();//The aim is to compact to each 8 bits into 1 byte
    if(isColored){
        btemp->size=len*3;
        btemp->data=malloc(len*3);//Théoriquement, on pourrait avoir un bitstream sur toute la plage de donné de l'image.

        for(int i=0;i<xmax;i++){
            for(int j=0;j<ymax;j++){
                if(Matrix[i+j*xmax]==1){
                    for(int k=0;k<3;k++){
                        btemp->data[randptr]=image[j][i][k]&0x01;//We read all the channels except alpha.
                        randptr++;
                    }
                }
                
            }
        }
        
        btemp2->size=len*3/8;
        btemp2->data=malloc(len*3/8);
        while(btemp->position<btemp->size){
            btemp2->data[count]+=((btemp->data[btemp->position])<<(7-bitpos));

            btemp->position++;
            bitpos++;
            if(bitpos>=8){
                bitpos=0;
                count++;
            }
        }
    }else{
        btemp->size=len;
        btemp->data=malloc(len);//Théoriquement, on pourrait avoir un bitstream sur toute la plage de donné de l'image.
        for(int i=0;i<xmax;i++){
            for(int j=0;j<ymax;j++){
                if(Matrix[i+j*xmax]==1){
                    btemp->data[randptr]=image[j][i][0]&0x01;//We use the red channel
                    randptr++;
                }
            }
        }
        btemp2->size=len/8;
        btemp2->data=malloc(len/8);

        while( (btemp->position) < (btemp->size) ){
            btemp2->data[count]+=((btemp->data[btemp->position])<<(7-bitpos));
            btemp->position++;
            bitpos++;



            if(bitpos>=8){
                bitpos=0;
                count++;
            }
        }
    }
    FreeBitstream(btemp);

    return btemp2;
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
