#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "color.h"

rgba hsv2rgb(hsv HSV)
{
  rgba RGB;
  double H = HSV.h, S = HSV.s, V = HSV.v;
  double P, Q, T, fraction;

  if (H >= 360)
  {
    H = 0;
  }
  else
  {
    H /= 60;
  }

  fraction = H - floor(H);
  P = V * (1 - S);
  Q = V * (1 - S * fraction);
  T = V * (1 - S * (1 - fraction));

  if (0 <= H && H < 1)
  {
    // RGB = (rgb){.r = V, .g = T, .b = P};
    RGB.r = (int)(V * 255.0);
    RGB.g = (int)(T * 255.0);
    RGB.b = (int)(P * 255.0);
  }
  else if (1 <= H && H < 2)
  {
    // RGB = (rgb){.r = Q, .g = V, .b = P};
    RGB.r = (int)(Q * 255.0);
    RGB.g = (int)(V * 255.0);
    RGB.b = (int)(P * 255.0);
  }
  else if (2 <= H && H < 3)
  {
    // RGB = (rgb){.r = P, .g = V, .b = T};
    RGB.r = (int)(P * 255.0);
    RGB.g = (int)(V * 255.0);
    RGB.b = (int)(T * 255.0);
  }
  else if (3 <= H && H < 4)
  {
    // RGB = (rgb){.r = P, .g = Q, .b = V};
    RGB.r = (int)(P * 255.0);
    RGB.g = (int)(Q * 255.0);
    RGB.b = (int)(V * 255.0);
  }
  else if (4 <= H && H < 5)
  {
    // RGB = (rgb){.r = T, .g = P, .b = V};
    RGB.r = (int)(T * 255.0);
    RGB.g = (int)(P * 255.0);
    RGB.b = (int)(V * 255.0);
  }
  else if (5 <= H && H < 6)
  {
    // RGB = (rgb){.r = V, .g = P, .b = Q};
    RGB.r = (int)(V * 255.0);
    RGB.g = (int)(P * 255.0);
    RGB.b = (int)(Q * 255.0);
  }
  else
  {
    // RGB = (rgb){.r = 0., .g = 0., .b = 0.};
    RGB.r = 0;
    RGB.g = 0;
    RGB.b = 0;
  }

  return RGB;
}

hsv rgb2hsv(rgba RGB)
{
  hsv ret;
  ret.h = 0;
  ret.s = 0;
  ret.v = 0;
  double min, max;
  double delta;
  double r, g, b;
  r = RGB.r / 255.0;
  g = RGB.g / 255.0;
  b = RGB.b / 255.0;
  if (r < g)
  {
    min = r;
  }
  else
  {
    min = g;
  }
  if (min < b)
  {
    min = min;
  }
  else
  {
    min = b;
  }
  if (r > g)
  {
    max = r;
  }
  else
  {
    max = g;
  }
  if (max > b)
  {
    max = max;
  }
  else
  {
    max = b;
  }
  ret.v = max;
  delta = max - min;
  if (delta < 0.00001)
  {
    ret.s = 0;
    ret.h = 0;
    return ret;
  }
  if (max > 0.0)
  {
    ret.s = (delta / max) * 100;
  }
  else
  {
    ret.s = 0.0;
    ret.h = 0.0;
    return ret;
  }
  if (r == max)
  {
    ret.h = fmod((60 * ((g - b) / delta) + 0), 360.0);
  }
  else
  {
    if (g == max)
    {
      ret.h = fmod((60 * ((b - r) / delta) + 120.0), 360.0);
    }
    else
    {

      ret.h = fmod((60 * ((r - g) / delta) + 240.0), 360.0);
    }
  }
  if (ret.h < 0.0)
  {
    ret.h += 360.0;
  }
  return ret;
}

unsigned char ***ConvertToRGB(unsigned char **data, int x_max, int y_max, int isColored)
{
  unsigned char ***RGB = malloc(y_max * sizeof(*RGB));
  for (int j = 0; j < y_max; j++)
  {
    RGB[j] = malloc(x_max * sizeof(*(RGB[j])));
    for (int i = 0; i < x_max; i++)
    {
      RGB[j][i] = malloc(4 * sizeof(*(RGB[j][i])));
    }
  }

  if (isColored)
  {
    for (int j = 0; j < y_max; j++)
    {
      for (int i = 0; i < 4 * x_max; i += 4)
      {
        RGB[j][i / 4][0] = data[j][i];
        RGB[j][i / 4][1] = data[j][i + 1];
        RGB[j][i / 4][2] = data[j][i + 2];
        RGB[j][i / 4][3] = data[j][i + 3];
      }
    }
  }
  else
  {
    for (int j = 0; j < y_max; j++)
    {
      for (int i = 0; i < x_max; i++)
      {
        RGB[j][i][0] = data[j][i];
        RGB[j][i][1] = data[j][i];
        RGB[j][i][2] = data[j][i];
        RGB[j][i][3] = 255;
      }
    }
  }
  return RGB;
}

unsigned char **ReconvertToPNG(unsigned char ***imageRAW, int x_max, int y_max, int isColored)
{
  unsigned char **data = malloc(y_max * sizeof(unsigned char *));
  if (isColored)
  {
    for (int i = 0; i < y_max; i++)
    {
      data[i] = malloc(4 * x_max * sizeof(unsigned char));
    }

    for (int j = 0; j < y_max; j++)
    {
      for (int i = 0; i < 4 * x_max; i += 4)
      {
        data[j][i] = imageRAW[j][i / 4][0];
        data[j][i + 1] = imageRAW[j][i / 4][1];
        data[j][i + 2] = imageRAW[j][i / 4][2];
        data[j][i + 3] = imageRAW[j][i / 4][3];
      }
    }
  }
  else
  {
    for (int i = 0; i < y_max; i++)
    {
      data[i] = malloc(x_max * sizeof(unsigned char));
    }

    for (int i = 0; i < x_max; i++)
    {
      for (int j = 0; j < y_max; j++)
      {
        data[j][i] = imageRAW[j][i][0];
      }
    }
  }

  return data;
}

/*
int main() {
  rgba RGB;
  RGB.r = 0;
  RGB.g = 0;
  RGB.b = 255;
  hsv HSV = rgb2hsv(RGB);
  printf("H:%f\tS:%f\tV:%f\n", HSV.h, HSV.s, HSV.v);
  return 0;
}
*/
