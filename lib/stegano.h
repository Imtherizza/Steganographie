#ifndef __STEGANO_H__
#define __STEGANO_H__

#include "message.h"
#include "color.h"
#include "../pngwrap.h"
#include "neobitstream.h"

int STEG_capacity(message *m,int imsize,int color);
int STEG_compatibility(const int len,const int capacity);
void STEG_write_from_decode(message* m);
int STEG_power(int a, int b);
bitstream* STEG_MessageToBitstream(message *m);
message * STEG_BitstreamToMessage(bitstream *b);


void ConvertColorToSTEG_IMG(STEG_IMG *NewIMG,rgba_image_t *image);
void ConvertBWToSTEG_IMG(STEG_IMG *NewIMG,bwimage_t *image);
void ConvertBackRGBAToPNG(STEG_IMG *EditedIMG,rgba_image_t *image);
void ConvertBackBWToPNG(STEG_IMG *EditedIMG,bwimage_t *image);
char* CreateOptionData(char* key,int message_size,int imgx,int imgy);
#endif//__STEGANO_H__