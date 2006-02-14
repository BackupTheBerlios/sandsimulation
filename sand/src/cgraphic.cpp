#include "cgraphic.h"
#include "iostream"




void cgraphic::Init()
{
    int i;
	ReadConfig();
    int flags = 0;
    bool fullscreen;
    Sint32 bpp;
    std::string Title;
    Sint32 width; 
    Sint32 height;
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        fprintf( stderr, "Video initialization failed: %s\n",
             SDL_GetError( ) );
        exit(1);
    }
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    flags = SDL_OPENGL;
    
    Config.Get("fullscreen",fullscreen);
    Config.Get("bits",bpp);
    Config.Get("AppName",Title);
    Config.Get("resx",width);
    Config.Get("resy",height);
	Config.Get("TimeIntervall",i);	TickIntervall=(unsigned long) i;
	Config.Get("ScreenSize",screen_size);
	Config.Get("CenterX",centerx);
	Config.Get("CenterY",centery);
    
    
     SDL_WM_SetCaption(Title.c_str(),Title.c_str());
    
    if (fullscreen==true)
       {
         flags+=SDL_FULLSCREEN;
       } else
       {
         flags+=SDL_SWSURFACE;
       }
    screen = SDL_SetVideoMode( width, height, bpp, flags );
    if( screen == 0 ) {
        fprintf( stderr, "Video mode set failed: %s\n",
             SDL_GetError( ) );
             exit( 1 );
            }
   
	#ifdef TESTSPEED
		TESTSPEED_VALUE=0.0;
        TESTSPEED_COUNT=0;
	#endif
 
}

void cgraphic::Quit()
{
    Config.Quit();
	SDL_Quit( );
	#ifdef TESTSPEED
		std::cout << "Durchschnittsgeschw: " << TESTSPEED_VALUE/(double)TESTSPEED_COUNT << std::endl;
        std::cout << "Werte: " << TESTSPEED_COUNT << std::endl;
	#endif
}

void cgraphic::setup_opengl( int width, int height )
{
    float ratio = (float) width / (float) height;
	

    /* Our shading model--Gouraud (smooth). */
    float visual_range;
    Config.Get("visual range",visual_range);
    
     /* Our shading model--Gouraud (smooth). */
    glShadeModel( GL_SMOOTH );

    /* Culling. */
    glCullFace( GL_BACK );
    glFrontFace( GL_CCW );
    glEnable( GL_CULL_FACE );
    glEnable(GL_DEPTH_TEST);
    

    /* Set the clear color. */
    glClearColor(0.0, 0.0, 0.2, 0 );

    /* Setup our viewport. */
    glViewport( 0, 0, width, height );

    /*
     * Change to the projection matrix and set
     * our viewing volume.
     */
    glDepthFunc(GL_LESS);
    
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    /*
     * EXERCISE:
     * Replace this with a call to glFrustum.
     */
    bool Orthmodeset;
	Config.Get("Orthogonal",Orthmodeset);
	if (Orthmodeset==true)
	  {
		glOrtho(ratio*(-screen_size)+centerx,ratio*screen_size+centerx,-screen_size+centery,screen_size+centery,-1.0,1.0);
	  }
	 else
	  {
		gluPerspective(50.0, ratio, 1.0, visual_range );
	  }
	//
    //InitScreen();
}

void cgraphic::Events()
{
    SDL_Event event;
    if (SDL_PollEvent( &event )!=0)
    {
        switch( event.type ) {
        case SDL_KEYDOWN:
            KeyEvents( &event.key.keysym );
            break;
        case SDL_VIDEORESIZE:
             {
             setup_opengl(event.resize.w,event.resize.h);
             DrawScreen( );
             break;
             }
        case SDL_QUIT:
            QuitEnable = true;
            break;
        }

    }
}

void cgraphic::KeyEvents( SDL_keysym* keysym )
{
    switch( keysym->sym ) {
    case SDLK_ESCAPE:
         QuitEnable=true;
        break;
    case SDLK_f:
         Swap_between_Fullscreen_And_Window();
         break;
	case SDLK_PLUS:
		 { 
	     int width,height;
		 Config.Get("resx",width);
         Config.Get("resy",height);
		 screen_size-=0.05;
	     setup_opengl(width,height);
		 break;
		 }
	case SDLK_MINUS:
		 { 
	     int width,height;
		 Config.Get("resx",width);
         Config.Get("resy",height);
		 screen_size+=0.05;
	     setup_opengl(width,height);
		 break;
		 }
	case SDLK_d:
		 { 
	     int width,height;
		 Config.Get("resx",width);
         Config.Get("resy",height);
		 centery+=0.2;
	     setup_opengl(width,height);
		 break;
		 }
	case SDLK_u:
		 { 
	     int width,height;
		 Config.Get("resx",width);
         Config.Get("resy",height);
		 centery-=0.2;
	     setup_opengl(width,height);
		 break;
		 }
	case SDLK_r:
		 { 
	     int width,height;
		 Config.Get("resx",width);
         Config.Get("resy",height);
		 centerx-=0.2;
	     setup_opengl(width,height);
		 break;
		 }
	case SDLK_l:
		 { 
	     int width,height;
		 Config.Get("resx",width);
         Config.Get("resy",height);
		 centerx+=0.2;
	     setup_opengl(width,height);
		 break;
		 }
	case SDLK_q:
		 {
			if (TickIntervall < 10)
			   {
				 TickIntervall=0;
			   } else
			   {
				 TickIntervall-=10;
			   }
			 break;
		 }
	case SDLK_s:
		 {
			 TickIntervall+=10;
			 break;
		 }	
    default:
        break;
    }
}

void cgraphic::Run()
{
     setup_opengl( screen->w, screen->h );
      while( !QuitEnable ) {
        /* Process incoming events. */
        Events( );
        /* Draw the screen. */
        DrawScreen( );
		SDL_Delay(TimeLeft());
        }
}

void cgraphic::Swap_between_Fullscreen_And_Window()
{
}

void cgraphic::InitScreen()
     {
       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       glFlush();
       SDL_GL_SwapBuffers( );
     }

void cgraphic::DrawScreen()
{
}

unsigned long cgraphic::TimeLeft()
{
  static unsigned long next_time=0;
  unsigned long now;
  now = SDL_GetTicks();
  #ifdef TESTSPEED
  static unsigned long counter=0;
   if (counter==100)
   {
	   TESTSPEED_VALUE+=(double)(now-next_time);
	   TESTSPEED_COUNT++;
	   next_time = now;
	   counter=0;
   }
   counter++;
   return 0;
  #endif
  #ifndef TESTSPEED
  if (next_time <= now)
  	{
	  next_time=now+TickIntervall;
	  return 0;
	}
  return(next_time-now);
  #endif
}

void cgraphic::SetConfigName(std::string _Name)
  {
     Config.Init(_Name);
  }

void cgraphic::GetConfigName(std::string &_Name)
  {       
     Config.GetFileName(_Name);
  }
  
void cgraphic::ReadConfig()
  {
    std::string S;
    Config.ReadFile();
    if (Config.Get("resx",S)==false) Config.Set("resx",(int)800);
    if (Config.Get("resy",S)==false) Config.Set("resy",(int)600);
    if (Config.Get("bits",S)==false) Config.Set("bits",(int)16);
    if (Config.Get("fullscreen",S)==false) Config.Set("fullscreen",(bool)false);
	if (Config.Get("Orthogonal",S)==false) Config.Set("Orthogonal",(bool)false);
	if (Config.Get("ScreenSize",S)==false) Config.Set("ScreenSize",(double)2.0);
	if (Config.Get("CenterX",S)==false) Config.Set("CenterX",(double)0.0);
	if (Config.Get("CenterY",S)==false) Config.Set("CenterY",(double)0.0);	
	if (Config.Get("TimeIntervall",S)==false) Config.Set("TimeIntervall",(int)10);
    if (Config.Get("AppName",S)==false) Config.Set("AppName","SDL_Anwenung");
    
    
  }
void cgraphic::WriteConfig()
  {
    Config.WriteFile();
  }
