#include "cSand.h"
#include <string>
#include <iostream>

using namespace std;

void cSand::Init()
{
  string S;
  int i,i2;
  Config->Get("umgebung",S);
  Config->Get("sandkoerner",count);
  umgvorgabe.LoadBMP((char*)S.c_str());
  umgvorgabe.ConvertTo(32);
  for (i=0;i!=umgvorgabe.width;i++)
     {
	   for (i2=0;i2!=umgvorgabe.height;i2++)
	   	{
		  if ((umgvorgabe.GetPixel(i,i2) > 0) && (umgvorgabe.GetPixel(i,i2) < (unsigned long)count))
		   {
			  umgvorgabe.SetPixel(i,i2,umgvorgabe.GetPixel(i,i2)+(10 << 16));
		   }
	   	}
	 }
  umgebung.CreateFrom(umgvorgabe);
  umgvorgabe.CreateTexture(cNotAntialiased);
  
  dt=0.1;
  koerner=new cVector[count];
  geschw =new cVector[count];
  
  srand (0);
  for (i=0;i!=count;i++)
     {
		double px,py;		 
		do
		  {
		   px=((double)rand ())/double(RAND_MAX)*double(umgebung.width-10)+5; 
		   py=((double)rand ())/double(RAND_MAX)*double(umgebung.height-10)+5;
		  } while (umgebung.GetPixel(int(px),int(py))!=0);
		umgebung.SetPixel(int(px),int(py),1000);
		koerner[i].x=px;
		koerner[i].y=py;
		float c;c=float(int((float)rand ()/float(RAND_MAX)*100.0)+100.0)/256;
		koerner[i].colorr=c;
		koerner[i].colorg=c;
		koerner[i].colorb=0;
	 }
  grav.x=0.0;
  grav.z=0.0;
  grav.y=-1.0;
  grav.r=-1.0;	
}

void cSand::NextStep()
{
  int i;
  unsigned long p1,p2;
  double addtox,addtoy;
	
  umgebung.Copy(umgvorgabe);
  for (i=0;i!=count;i++)
    {
	   if (umgebung.GetPixel(int(koerner[i].x),int(koerner[i].y))==0)
		   umgebung.SetPixel(int(koerner[i].x),int(koerner[i].y),i);
	}
  for (i=0;i!=count;i++)
    {
	  // x=v0t + a/2t^2 
	 
	  addtox=dt*geschw[i].x+grav.x/2.0*dt*dt+koerner[i].x;	
	  
	  {
	  p1=umgebung.GetPixel(int(addtox),int(koerner[i].y));
	  
	
	  if (((p1==0)||(p1==(unsigned long) i)))
	     {
	        koerner[i].x=addtox;
	        // v=v0+at;
	        geschw[i].x=geschw[i].x+grav.x*dt;
	     } else
	     {
		   geschw[i].x=0.0;
	     }
	  }
	  	   
	  addtoy=dt*geschw[i].y+grav.y/2.0*dt*dt+koerner[i].y;
	  
	  {
	  p2=umgebung.GetPixel(int(koerner[i].x),int(addtoy));	
	  if ((p2==0)||(p2==(unsigned long) i))
	    { 
	      koerner[i].y=addtoy;
	      // v=v0+at;
	      geschw[i].y=geschw[i].y+grav.y*dt;
	     } else
	    { 
		  geschw[i].y=0.0; 
	    }
	   }
    }
 
}

void cSand::NextStep2()
{
  int i,i2;
  unsigned long p1,p2;
  double addtox,addtoy;
  double dt_part;
  int anz;
  //dt=5.0;
  if (grav.x==0.0) grav.x=-0.0001;
  if (grav.y==0.0) grav.y=-0.0001;
  umgebung.Copy(umgvorgabe);
  for (i=0;i!=count;i++)
    {
	   if (umgebung.GetPixel(int(koerner[i].x),int(koerner[i].y))==0)
		   umgebung.SetPixel(int(koerner[i].x),int(koerner[i].y),i);
	}
  for (i=0;i!=count;i++)
    {
	  // x=v0t + a/2t^2 
	 
	  dt_part=fabs((-geschw[i].x-sqrt(geschw[i].x*geschw[i].x-2*grav.x))/grav.x);
	  //cout << dt_part << endl;
	  anz=int(ceil(5.0/dt_part));
	  dt_part = 5.0/double(anz);
	  dt=dt_part;
	  for (i2=0;i2!=anz;i2++)
	  {	  
	  addtox=dt*geschw[i].x+grav.x/2.0*dt*dt+koerner[i].x;	
	  
	  {
	  p1=umgebung.GetPixel(int(addtox),int(koerner[i].y));
	  
	
	  if (((p1==0)||(p1==(unsigned long) i)))
	     {
	        
			 koerner[i].x=addtox;
	        // v=v0+at;
	        geschw[i].x=geschw[i].x+grav.x*dt;
	     } else
	     {
		   geschw[i].x=0.0;
	     }
	  }
  	  }//i2
	  	   
	  // DELTA_Y = v_y*dt + g/2(dt)^2 = -1
	  // -> dt = (-v_y +- sqrt(v_y^2-2g))/g
	  // =(-v_y + (sqrt(2g)+1/2*1/sqrt(2g)*v_y^2)/g
	  
	  
	  
	  dt_part=fabs((-geschw[i].y-sqrt(geschw[i].y*geschw[i].y-2*grav.y))/grav.y);
	  //cout << dt_part << endl;
	  anz=int(ceil(5.0/dt_part));
	  dt_part = 5.0/double(anz);
	  dt=dt_part;
	  for (i2=0;i2!=anz;i2++)
	  {	  
	  
	  
	  addtoy=dt*geschw[i].y+grav.y/2.0*dt*dt+koerner[i].y;
	  
	  {
	  p2=umgebung.GetPixel(int(koerner[i].x),int(addtoy));	
	  if ((p2==0)||(p2==(unsigned long) i))
	    { 
		  koerner[i].y=addtoy;
	      // v=v0+at;
	      geschw[i].y=geschw[i].y+grav.y*dt;
	     } else
	    { 
		  geschw[i].y=0.0; 
	    }
	   }
   	  }//i2
    }

}


void cSand::Quit()
{
count =0;
if (koerner!=NULL)delete koerner;
if (geschw!=NULL) delete geschw;
}
