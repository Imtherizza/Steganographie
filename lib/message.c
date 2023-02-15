#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "lib/message.h"

message* CreateFromFile(char *filename, FILE *file);
{
    const char t_s_filename = sizeof(filename)*sizeof(char);
    const char f_format = ".png";
    message* r_message = malloc(sizeof(message));
    r_message->size_of_filename = t_s_filename;
    r_message->filename = malloc(t_s_filename);

    //il doit y avoir un meilleur moyen pour ca quand meme
    //data extract, png only rn
    if(f_format==".png")
    {
        bwimage_t* t_image=E3ACreateImage();
        r_message->size_of_data = malloc(sizeof(char)*image->height*image->width);
        for(int i=0; i<t_image->height; i++){
            for(int j=0; j<t_image->width; j++){
                r_message->data[i+j*image->width]=image->data[i][j];
            }
        };
    }
    
    r_message->crc = ComputeCRC(r_message);

}

unsigned int ComputeCRC(message* m)
{

}

message* Reallocate(message *m)
{

}

void FreeMessage(message *m)
{
    if(m->filename)
        free(m->filename);
    if(m->data)
        free(m->data);
    free(m);
}