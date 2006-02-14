#include "cSurface.h"
#include <fstream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>


bool cPalette::Create(int _nColors)
  {
    if (colors==NULL) return false;
    ncolors=_nColors;
    colors = new cColor[ncolors];
    return true;
  }
  
bool cPalette::Delete()
  {
    if (colors==NULL) return false;
    delete(colors);
    colors=NULL;
    ncolors=0;    
	return true;
  }


bool cSurface::Create(int _width,int _height,int _bpp)
  {
    if (pixels!=NULL) return false;
    height=_height;
    width=_width;
    bpp=_bpp;
    ByteOrder=cRGB;
    switch (bpp)
      { 
        case 8  : pixels=new char[width*height];bytesperpixel=1; break;
        case 16 : pixels=new char[width*height*2];bytesperpixel=2;break;
        case 24 : pixels=new char[width*height*3];bytesperpixel=3;break;
        case 32 : pixels=new char[width*height*4];bytesperpixel=4;break;
        default : return false;
      }
    ByteOrder=cRGB;
    return true;
  }
  
bool cSurface::Delete()
  {
    if (pixels==NULL) return false;
    delete((char*)pixels);
    if (Palette!=NULL) delete Palette;
    Palette=NULL;
    pixels=NULL;
  return true;
  }
  

  

bool cSurface::ClearSurface(unsigned int Color)
  {
  int i3;
   switch (bpp)
     {
       case 8  : memset(pixels,(unsigned char) Color,width*height); break;
       case 16 : for (i3=0;i3!=width*height;i3++)
                   {
                      ((unsigned short*) pixels)[i3] = (unsigned short) Color;
                   }
                   break;
       case 24 : for (i3=0;i3!=width*height*3;i3+=3)
                   {
                      ((unsigned char*) pixels)[i3] = (unsigned char) Color;
                      ((unsigned char*) pixels)[i3+1] = ((unsigned char*)(&Color))[1];
                      ((unsigned char*) pixels)[i3+2] = ((unsigned char*)(&Color))[2];
                   }
                   break;
       case 32:  for (i3=0;i3!=width*height;i3++)
                   {
                      ((unsigned long*) pixels)[i3] = (unsigned long) Color;
                   }
                   break;
     }
  return true;   
  }
  
unsigned long cSurface::GetPixel(int x, int y)
{
    switch (bpp)
    {
        case 8:
            return ((unsigned char*) pixels)[y*width+x]; break;

        case 16:
            return ((unsigned short*) pixels)[y*width+x]; break;
        case 24:
            return ((unsigned char*) pixels)[(y*width+x)*3] | ((unsigned char*) pixels)[(y*width+x)*3+1] << 8| ((unsigned char*) pixels)[(y*width+x)*3+2] << 16;
            break;
        case 32:
            return ((unsigned long*) pixels)[y*width+x]; break;

        default:
            return 0;
    }
}

void cSurface::SetPixel(int x, int y, unsigned long Pixel)
{
    switch (bpp)
    {
        case 8:
            ((unsigned char*) pixels)[y*width+x] = (unsigned char) Pixel;break;

        case 16:
            ((unsigned short*) pixels)[y*width+x]= (unsigned short) Pixel;break;
        case 24:
            ((unsigned char*) pixels)[(y*width+x)*3] = (unsigned char) Pixel;
            ((unsigned char*) pixels)[(y*width+x)*3+1] = ((unsigned char*)(&Pixel))[1];
            ((unsigned char*) pixels)[(y*width+x)*3+2] = ((unsigned char*)(&Pixel))[2];
            break;
        case 32:
            ((unsigned long*) pixels)[y*width+x] = (unsigned long) Pixel; break;
    }
}

void cSurface::ChangeByteOrder()
{
   int i;
   unsigned char b;
   switch (bpp)
   {
   case 24:  {
               for (i = 0; i!= width*height;i++)
                   {
                    b = ((unsigned char*) pixels)[i*3];
                        ((unsigned char*) pixels)[i*3] = ((unsigned char*) pixels)[i*3+2];;
                        ((unsigned char*) pixels)[i*3+2] = b;
                    }
               if (ByteOrder==cRGB) {ByteOrder=cBGR;} else {ByteOrder=cRGB;};
               } break;
   case 32:  {
               for (i = 0; i!= width*height;i++)
                   {
                    b = ((unsigned char*) pixels)[i*4];
                        ((unsigned char*) pixels)[i*4] = ((unsigned char*) pixels)[i*4+2];;
                        ((unsigned char*) pixels)[i*4+2] = b;
                    }
               if (ByteOrder==cRGB) {ByteOrder=cBGR;} else {ByteOrder=cRGB;};
               } break;
     }
     
}
  
GLuint cSurface::CreateTexture(int Antialiased)
  {
    GLuint ColorFormat=0;
    switch (bpp)
      {
        case 8: ColorFormat=GL_COLOR_INDEX; break;
        case 16: ColorFormat=0; break;
        case 24:ColorFormat= GL_RGB; break;
        case 32: ColorFormat=GL_RGBA; break;
      }
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D,texture);
    switch (Antialiased)
      {
        case cNotAntialiased : 
                           glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
                           glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
                           glTexImage2D(GL_TEXTURE_2D,0,ColorFormat,width,height,0,ColorFormat,GL_UNSIGNED_BYTE,pixels);
                           break;
        case cAntialiased    : 
                           glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
                           glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_LINEAR);    
                           gluBuild2DMipmaps(GL_TEXTURE_2D,ColorFormat,width,height,ColorFormat,GL_UNSIGNED_BYTE,pixels); 
                           break;
      }
    return texture;
  }
  
GLvoid cSurface::PaintTexture(float _x, float _y, float _z)
  {
     glPushMatrix();
     glTranslatef(_x,_y,_z);
     glBindTexture(GL_TEXTURE_2D,texture);
     glBegin(GL_QUADS);
       glTexCoord2f(0.0,0.0);glVertex3f(-0.5,-0.5,0.0);
       glTexCoord2f(1.0,0.0);glVertex3f(0.5,-0.5,0.0);
       glTexCoord2f(1.0,1.0);glVertex3f(0.5,0.5,0.0);
       glTexCoord2f(0.0,1.0);glVertex3f(-0.5,0.5,0.0);
     glEnd();
     glPopMatrix();
  }


bool cSurface::LoadBMP(char *filename)
  {
    // Header file of BMP-FILE
    unsigned short  bfType; // must be 0x4d42;
    unsigned long bfSize;
    unsigned short  bfReserved1;
    unsigned short  bfReserved2;
    unsigned long bfOffBits;
    //
    unsigned long biSize;
    unsigned long biWidth;
    unsigned long biHeight;
    unsigned short biPlanes;
    unsigned short  biBitCount;
    unsigned long biCompression;
    unsigned long biSizeImage;
    unsigned long biXPelsPerMeter;
    unsigned long biYPelsPerMeter;
    unsigned long biClrUsed;
    unsigned long biClrImportant;
    std::ifstream inf;
    inf.open(filename, std::ios::in | std::ios::binary);
    inf.read((char*)&bfType,2); if (bfType!=0x4d42) {inf.close(); return false;}
    inf.read((char*)&bfSize,4);
    inf.read((char*)&bfReserved1,2);
    inf.read((char*)&bfReserved2,2);
    inf.read((char*)&bfOffBits,4);
    
    inf.read((char*)&biSize,4);
    inf.read((char*)&biWidth,4);
    inf.read((char*)&biHeight,4);
    inf.read((char*)&biPlanes,2);
    inf.read((char*)&biBitCount,2);
    inf.read((char*)&biCompression,4);
    inf.read((char*)&biSizeImage,4);
    inf.read((char*)&biXPelsPerMeter,4);
    inf.read((char*)&biYPelsPerMeter,4);
    inf.read((char*)&biClrUsed,4);
    inf.read((char*)&biClrImportant,4);
    if (biCompression!=0) {inf.close(); return false;}
    if (Create(biWidth,biHeight,(int)biBitCount)==false) {inf.close(); return false;}
    if (biBitCount==8)
      {
        
      }
    switch (bpp)
      {
       case 8:
                Palette=new cPalette();
                Palette->Create(256);
                inf.read((char*)Palette->colors,256*4);
                inf.read((char*)pixels,height*width);
                break;
       case 16:
                inf.read((char*)pixels,height*width*2);
                break;
       case 24:     
                inf.read((char*)pixels,height*width*3);
                break;
       case 32:
                inf.read((char*)pixels,height*width*4);
                break;                
       }
    inf.close();
    ByteOrder=cBGR;
    ChangeByteOrder();
    return true;
  }
  
bool cSurface::LoadTGA(char *filename)
  {
  unsigned char TGAcomment[1];                            // Nr. of bytes used for the comment
  unsigned char TGAheader[11]={0,2,0,0,0,0,0,0,0,0,0}; // Uncompressed TGA Header
  unsigned char header[6];                             // First 6 Useful Bytes From The Header
//  unsigned long  imageSize;                             // Used To Store The Image Size When Setting Aside Ram

  
  std::ifstream inf;
  inf.open(filename, std::ios::in | std::ios::binary);
  inf.read((char *)TGAcomment,1); // read bytes of comments of the file
  inf.read((char *)TGAheader,11); // read useless header (only for identify tga file)
  inf.read((char *)header,6); //read header file
  //if comments then read them:
  if (TGAcomment[0] != 0)
    {
      char comment[TGAcomment[0]];
      inf.read((char *)comment,sizeof(comment));
    }
  //calculate width and height:
  width = header[1]*256+header[0];
  height = header[3] * 256+header[2];
  bpp = header[4];
  if (Create(width,height,bpp)==false) {inf.close();return false;}
  switch (bpp)
      {
       case 8:
                Palette=new cPalette();
                Palette->Create(256);
                inf.read((char*)Palette->colors,256*4);
                inf.read((char*)pixels,height*width);
                break;
       case 16:
                inf.read((char*)pixels,height*width*2);
                break;
       case 24:     
                inf.read((char*)pixels,height*width*3);
                break;
       case 32:
                inf.read((char*)pixels,height*width*4);
                break;                
       }
  
  inf.close();
  ByteOrder=cBGR;
  ChangeByteOrder();
  return true;
  }
  
void cSurface::CreateRGBNoise(unsigned char rl,unsigned char rh,unsigned char gl,unsigned char gh,unsigned char bl,unsigned char bh)
  {
     srand (time (0));
     int i,i2;
     unsigned long r,g,b;
     for (i2=0;i2!=height;i2++)
       {
         for (i=0;i!=width;i++)
           {
             r=(unsigned int)(double(rl)+double(rand ())/double(RAND_MAX)*double(rh-rl));
             g=(unsigned int)(double(gl)+double(rand ())/double(RAND_MAX)*double(gh-gl));
             b=(unsigned int)(double(bl)+double(rand ())/double(RAND_MAX)*double(bh-bl));
             SetPixel(i,i2,r+(g<<8)+(b<<16));
           }
       }
  }
  
void cSurface::CreateRNoise(unsigned char rl,unsigned char rh,unsigned int _seed)
  {
     srand (_seed);
     int i,i2;
     unsigned long r,g,b;
     unsigned char ch;
     for (i2=0;i2!=height;i2++)
       {
         for (i=0;i!=width;i++)
           {
             r=(unsigned int)(double(rl)+double(rand ())/double(RAND_MAX)*double(rh-rl));
             ch = (unsigned char)((GetPixel(i,i2)>>8));
             g=(unsigned long)(ch);
             ch = (unsigned char)((GetPixel(i,i2)>>16));
             b=(unsigned long)(ch);
             SetPixel(i,i2,r+(g<<8)+(b<<16));
           }
       }
  }
  
void cSurface::CreateGNoise(unsigned char gl,unsigned char gh,unsigned int _seed)
  {
     srand (_seed);
     int i,i2;
     unsigned long r,g,b;
     unsigned char ch;
     for (i2=0;i2!=height;i2++)
       {
         for (i=0;i!=width;i++)
           {
             g=(unsigned int)(double(gl)+double(rand ())/double(RAND_MAX)*double(gh-gl));
             ch = (unsigned char)((GetPixel(i,i2)));
             r=(unsigned long)(ch);
             ch = (unsigned char)((GetPixel(i,i2)>>16));
             b=(unsigned long)(ch);
             SetPixel(i,i2,r+(g<<8)+(b<<16));
           }
       }
  }
  
void cSurface::CreateBNoise(unsigned char bl,unsigned char bh,unsigned int _seed)
  {
     srand (_seed);
     int i,i2;
     unsigned long r,g,b;
     unsigned char ch;
     for (i2=0;i2!=height;i2++)
       {
         for (i=0;i!=width;i++)
           {
             b=(unsigned int)(double(bl)+double(rand ())/double(RAND_MAX)*double(bh-bl));
             ch = (unsigned char)((GetPixel(i,i2)));
             r=(unsigned long)(ch);
             ch = (unsigned char)((GetPixel(i,i2)>>8));
             g=(unsigned long)(ch);
             SetPixel(i,i2,r+(g<<8)+(b<<16));
           }
       }
  }
  
  
  void cSurface::Copy(cSurface &Source)
  {
	//memcpy(void *dest, const void *src, size_t n);
     memcpy(pixels,Source.pixels,width*height*bytesperpixel);
  }
  
  void cSurface::CreateFrom(cSurface &Source)
  {
	 Create(Source.width,Source.height,Source.bpp);
	 Copy(Source);
  }
  
  bool cSurface::ConvertTo(int _bpp)
  {
	 switch (_bpp)
	 {
		 case 32:
		    {
			  switch (bpp)
			  		{
					case  16:			       
 					case  24: {
					   		  cSurface temp;
					          int i,i2;
					          temp.Create(width,height,32);
					          for (i2=0;i2!=height;i2++)
							  	{
								  for (i=0;i!=width;i++)
								    {
									  temp.SetPixel(i,i2,GetPixel(i,i2));
									}
								}
							  bytesperpixel=temp.bytesperpixel;
							  bpp=temp.bpp;
							  delete((char*)pixels);
							  pixels=new char[width*height*4];
							  Copy(temp);									
					          temp.Delete();
				   		   	  break;
					   		}
				   case 32: break;
				   default: return false;
				   }
			  break;
			}
		 default:
		 return false;
	 }
	 return true;
  }
