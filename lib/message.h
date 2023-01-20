#ifndef __MESSAGE_H__
#define __MESSAGE_H__

typedef struct{
unsigned int crc;
unsigned char size_of_filename;
unsigned int size_of_data;
char *filename;
char *data;
}message;
message* CreateFromFile(char *filename, FILE *file);
unsigned int ComputeCRC(message* m);
message* Reallocate(message *m);
void FreeMessage(message *m);

#endif//__MESSAGE_H__