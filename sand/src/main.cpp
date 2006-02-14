// 10.02.2006 written by Christian Klein
#include <GL/gl.h>
#include <GL/glu.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cmath>
#include "cgraphic.h"
#include "cSurface.h"
#include "cSand.h"

using namespace std;


class myopengl : public cgraphic
{
   public:
          myopengl(){Winkel=0.0;};
          virtual ~myopengl(){};
          void Init();
          void Quit();
          
   protected:
   void DrawScreen();
   void KeyEvents( SDL_keysym* keysym );
   cSand Sand;
   double Winkel;
   bool automatic;
   
};

void myopengl::Init()
{
cgraphic::Init();
Sand.Config = &Config;
Sand.Init();
automatic=false;
//glPointSize(2.0);
//glEnable(GL_POINT_SMOOTH);
}

void myopengl::Quit()
{
cgraphic::Quit();
}

void myopengl::DrawScreen()
{
   static int zeitschritt=0;
   
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   
   
	
   glRotated(Winkel,0.0,0.0,-1.0);
   
   // Paint the sand corns:
     glPushMatrix();
         glTranslatef(-0.5,-0.5,0.0);
         glDisable(GL_TEXTURE_2D);
           glBegin(GL_POINTS);
              for (int i=0;i!=Sand.count;i++)
	              {
		           glColor3f(Sand.koerner[i].colorr,Sand.koerner[i].colorg,Sand.koerner[i].colorb);  
		           glVertex2d(((Sand.koerner[i].x))/256.0,(Sand.koerner[i].y)/256.0);
	              }
           glEnd();
     glPopMatrix();
   
   //Paint the Background Texture:
   glColor3f(1.0,1.0,1.0);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,Sand.umgvorgabe.texture);
     glBegin(GL_QUADS);
       glTexCoord2f(0.0,0.0);glVertex2f(-0.5,-0.5);
       glTexCoord2f(1.0,0.0);glVertex2f(0.5,-0.5);
       glTexCoord2f(1.0,1.0);glVertex2f(0.5,0.5);
       glTexCoord2f(0.0,1.0);glVertex2f(-0.5,0.5);
     glEnd();
   
   
   //Show everything to screen:
   SDL_GL_SwapBuffers();
   
   
   //Calculate next step:
   Sand.NextStep();
				  
   
   // if space is pressed: screen rotates automatic after 1000 steps
   if (automatic==true)
   {
   zeitschritt++;
   if (zeitschritt==500)
   {
	   zeitschritt=0;
	   Winkel=(double)rand ()/double(RAND_MAX)*double(360.0);
	   Sand.grav.x=-sin(Winkel*(M_PI/180.0))*Sand.grav.r;
	   Sand.grav.y=cos(Winkel*(M_PI/180.0))*Sand.grav.r;
   }
   }
  
}

void myopengl::KeyEvents( SDL_keysym* keysym )
{
  cgraphic::KeyEvents(keysym);
  if (keysym->sym==SDLK_RIGHT)
   {
	   Winkel+=360/36;
	   Sand.grav.x=-sin(Winkel*(M_PI/180.0))*Sand.grav.r;
	   Sand.grav.y=cos(Winkel*(M_PI/180.0))*Sand.grav.r;
   }
   
   if (keysym->sym==SDLK_LEFT)
   {
	   Winkel-=360/36;
	   Sand.grav.x=-sin(Winkel*(M_PI/180.0))*Sand.grav.r;
	   Sand.grav.y=+cos(Winkel*(M_PI/180.0))*Sand.grav.r;
   }
   if (keysym->sym==SDLK_SPACE)
   {
	  automatic=!automatic;
   }
   if (keysym->sym==SDLK_PLUS)
   {
	  
   }

}


int main( int argc, char* argv[] )
{
    myopengl Graph;
    Graph.Init();
    Graph.Run();
    Graph.Quit();
    
    return 0;
}
