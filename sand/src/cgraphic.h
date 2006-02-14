// written by Christian Klein
//graphik: change:30.dez.2005
// change 18.01.2006

// 12. Feb. 2006: change: now able to draw orthogonal graphic
//						  and resize with + and -
//						  with u,d,r,l you can center it
//					      add time intevall:
//						  q-> quicker; s -> slower
// config.txt example -> end of file

#ifndef __CGRAPHIC_H__
#define  __CGRAPHIC_H__

#include <string>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include "configtool.h"

//define TESTSPEED

class cgraphic
      {
        public:
        cgraphic() {QuitEnable = false;SetConfigName("config.txt");};
        virtual ~cgraphic(){};
virtual void Init();
virtual void Run();
virtual void Quit();
       
        void SetConfigName(std::string _Name);
        void GetConfigName(std::string &_Name);

        protected:
virtual void setup_opengl( int width, int height );
virtual void Swap_between_Fullscreen_And_Window();
virtual void Events();
virtual void KeyEvents( SDL_keysym* keysym );
virtual void InitScreen();
virtual void DrawScreen();

virtual void ReadConfig();
virtual void WriteConfig(); 
		
unsigned long TimeLeft();

bool  QuitEnable;
SDL_Surface *screen;
ConfigTool Config;
double screen_size;
double centerx,centery;
unsigned long TickIntervall; // time in millisec
#ifdef TESTSPEED
double TESTSPEED_VALUE;
unsigned long TESTSPEED_COUNT;
#endif
      };

#endif
	  
// Example of config.txt:
/*
resx=800
resy=600
bits=24
fullscreen=0
Orthogonal=1
ScreenSize=0.6
CenterX=0.0
CenterY=0.0
TimeIntervall=10
datapath=/
execname=MyApplication
AppName=MyApplication
*/
