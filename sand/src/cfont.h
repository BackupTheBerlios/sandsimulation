// by Christian Klein
// 15.Dec 2005

#ifndef __CFONT_H__
#define  __CFONT_H__

#include <stdio.h>
#include <stdlib.h>
#include "cSurface.h"
#include <GL/gl.h>


class cFont
  {
    public:
    cFont(){fontbitmap=NULL;SetSize(1.0,1.0);SetTextSpace(1.0);SetTextStyle(0);};
    ~cFont(){Delete();};
    
bool Load(char *filename);
void SetSize(float _sizex,float _sizey);
void SetTextSpace(float _space);
void SetTextStyle(unsigned int _style);
void Write(float x, float y,float z, char *TextStr);

void Delete();
    
    protected:
void WriteChar(char ch);

cSurface *fontbitmap;
GLuint lletters;
float sizex,sizey;
float space;
unsigned int style;
  };

#endif
