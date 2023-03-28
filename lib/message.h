#ifndef __MESSAGE_H__
#define __MESSAGE_H__

typedef struct
{
    unsigned int crc;
    unsigned char size_of_filename;
    unsigned int size_of_data;
    unsigned int size_of_option;
    char *filename;
    unsigned char *data;
    // char *option; Deja inclu dans data.
} message;

// non en cours
message *CreateFromFile(const char *filepath);

// non
unsigned int ComputeCRC(message *m);

// fait
void FreeMessage(message *m);

//[JC]
void InjectOptionIntoData(message *m, char *option);

//[JC]
void DumpImageFromMessage(message *m);

#endif //__MESSAGE_H__