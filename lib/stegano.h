#ifndef __STEGANO_H__
#define __STEGANO_H__

#define NO_COLOR 0x00
#define COLOR 0x01

size_t STEG_extract_file(unsigned char* ex_data,unsigned char* filepath);

unsigned int STEG_est_max_in_img(unsigned char* image_data,int datasize,int color);

unsigned char* STEG_header_creation(FILE* file,unsigned char* filepath,size_t* filesize);

void STEG_write_bit(unsigned char* data,unsigned char byte,unsigned int position);

void STEG_process_GRAY_linear(unsigned char* ex_data,bwimage_t* image);

void STEG_process_RGB_linear(unsigned char* ex_data,rgba_image_t* image);

#endif//__STEGANO_H__