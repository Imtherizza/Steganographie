#ifndef __STEGANO_H__
#define __STEGANO_H__

void STEG_extract_file(unsigned char* ex_data,unsigned char* filepath);

void STEG_est_max_in_img(unsigned char* image_data);

void STEG_write_bit(unsigned char* data,unsigned char byte);

void STEG_process(unsigned char* ex_data,bwimage_t* image);

#endif//__STEGANO_H__