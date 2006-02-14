#include "cfont.h"
#include <string.h>


bool cFont::Load(char *filename)
  {
    int i,i2,i3;
    //Try to load as a BMP - file:
    fontbitmap=new cSurface();
    if (fontbitmap->LoadBMP(filename)==false)
      {
         // maybe it is a TGA - file:
         if (fontbitmap->LoadTGA(filename)==false)
           {
             Delete();
             return false;
           }
      }
    fontbitmap->CreateTexture(cAntialiased);
    lletters=glGenLists(256);
       i3=0;
       for (i2=15;i2!=-1;i2--)
         {
           for (i=0;i!=16;i++)
             {
                glNewList(lletters+i3,GL_COMPILE);
                glBindTexture(GL_TEXTURE_2D,fontbitmap->texture);
                  i3++;
                    glBegin(GL_QUADS);
                      glTexCoord2f((float)i/16.0,(float)i2/16.0);glVertex3f(0.0,0.0,0.0);
                      glTexCoord2f((float)(i+1)/16.0,(float)i2/16.0);glVertex3f(1.0,0.0,0.0);
                      glTexCoord2f((float)(i+1)/16.0,(float)(i2+1)/16.0);glVertex3f(1.0,1.0,0.0);
                      glTexCoord2f((float)i/16.0,(float)(i2+1)/16.0);glVertex3f(0.0,1.0,0.0);
                    glEnd();
                glEndList();
               
             }
        }
   return true; 
  }
  
void cFont::SetSize(float _sizex,float _sizey)
  {
    sizex=_sizex/10.0;
    sizey=_sizey/10.0;
  }
  
void cFont::SetTextStyle(unsigned int _style)
  {
    style=_style;
  }
  
void cFont::SetTextSpace(float _space)
  {
    space=_space;
  }
  
void cFont::Write(float x, float y,float z,char *TextStr)
  {
  int i;
    glPushMatrix();
      glScalef(sizex,sizey,1.0);
      glTranslatef(x,y,z);
      for (i=0;i!=(int)strlen(TextStr);i++)
        {
          glTranslatef(space,0.0,0.0);
          glCallList(lletters+TextStr[i]-32+128*style);
        }
    glPopMatrix();
  }

void cFont::WriteChar(char ch)
  {
    //glCallList(lletter+ch);
  }

void cFont::Delete()
  {
    if (fontbitmap!=NULL)
      {
        delete fontbitmap;
        fontbitmap=NULL;
      }
  }
