#ifndef __MESSAGE_H__
#define __MESSAGE_H__

typedef struct{
    unsigned int crc;
    unsigned char size_of_filename;
    unsigned int size_of_data;
    char *filename;
    char *data;
}message;

//non en cours
message* CreateFromFile(const char *filepath);

//non
unsigned int ComputeCRC(message* m);

//non
message* Reallocate(message *m);

//fait
void FreeMessage(message *m);

#endif//__MESSAGE_H__