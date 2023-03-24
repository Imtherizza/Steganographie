#ifndef __STEGANO_H__
#define __STEGANO_H__

#include "message.h"

size_t STEG_get_size(const char* filepath);
size_t STEG_extract_file(unsigned char* ex_data);
void STEG_extract_message(char* ex_data,message* m);
int STEG_capacity(message *m,int imsize,int color);
int STEG_compatibility(const int len,const int capacity);
void STEG_write_bits_lin(char* im_data,char ex_byte,int* pos);
void STEG_CV_process(const char* im_data);
void STEG_LSB_capture_lin(const int img_size,const char* f_data,char* c_data);
message* STEG_capture_decode(const char* in_data);
void STEG_write_from_decode(message* m);
int STEG_power(int a, int b);

#endif//__STEGANO_H__