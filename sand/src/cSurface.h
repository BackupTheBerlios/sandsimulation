// by Christian Klein
// 15.Dec 2005
#ifndef __CSURFACE_H__
#define  __CSURFACE_H__

#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>

enum {cRGB=0, cBGR};
enum {cNotAntialiased=0,cAntialiased};

class cColor { public: unsigned char r; unsigned char g; unsigned char b; unsigned char a;};

class cPalette
  {
    public:
     cPalette() {colors=NULL;ncolors=0;}
    ~cPalette() {Delete();}
     bool Create(int _nColors);
     bool Delete();
     int ncolors;
     cColor *colors;
  };


class cSurface 
      {
      public:
              cSurface(){pixels=NULL;width=0;height=0;bpp=0;Palette=NULL;texture=0;ByteOrder=cRGB;};
              ~cSurface(){Delete();};
bool          Create(int _width,int _height,int _bpp);
bool          Delete();
bool          ClearSurface(unsigned int Color);
unsigned long GetPixel(int x, int y);
void          SetPixel(int x, int y, unsigned long Pixel);
void          ChangeByteOrder();
bool          LoadBMP(char *filename);
bool          LoadTGA(char *filename);
void          CreateRGBNoise(unsigned char rl,unsigned char rh,unsigned char gl,unsigned char gh,unsigned char bl,unsigned char bh);
void          CreateRNoise(unsigned char rl=0,unsigned char rh=255,unsigned int _seed=0);
void          CreateGNoise(unsigned char gl=0,unsigned char gh=255,unsigned int _seed=0);
void          CreateBNoise(unsigned char bl=0,unsigned char bh=255,unsigned int _seed=0);
void 		  CreateFrom(cSurface &Source);
void 		  Copy(cSurface &Source); //Copy Surface from Source should have same properties			
bool 		  ConvertTo(int _bpp); //not ready: only converts 16/24 to 32 bit 



// OPENGL Procedures:
          
GLuint       CreateTexture(int Antialiased=cNotAntialiased);
GLvoid       PaintTexture(float _x, float _y, float _z);// only for testing
      
      void *pixels;
      int bpp;
	  int bytesperpixel;
      int height;
      int width;
      int ByteOrder;
      GLuint texture;
      cPalette *Palette;
      };

#endif
