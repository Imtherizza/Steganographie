#ifndef __HSV_H__
#define __HSV_H__

typedef struct {
    unsigned char r;      //0-255
    unsigned char g;      //0-255
    unsigned char b;      //0-255
    unsigned char a;      //0-255
} rgba;

typedef struct
{
    int xmax;
    int ymax;
    unsigned char*** RGB;
}STEG_IMG;



typedef struct {
    double h;       // 0-360
    double s;       // 0-1
    double v;       // 0-1
} hsv;

//Converti HSV vers RGB.
//Fait
rgba hsv2rgb(hsv HSV);
//Converti RGB vers HSV.
//Fait
hsv rgb2hsv(rgba RGB);


unsigned char*** ConvertToRGB(unsigned char** data, int x_max, int y_max, int isColored);
unsigned char** ReconvertToPNG(unsigned char*** imageRAW,int x_max,int y_max,int isColored);
#endif
