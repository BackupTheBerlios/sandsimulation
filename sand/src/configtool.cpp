#include "configtool.h"
#include <fstream>
#include <iostream>

using namespace std;

void ConfigTool::Init(std::string _filename)
  {
    filename=_filename;
  }
  
void ConfigTool::GetFileName(std::string &_filename)
  {
    _filename=filename;
  }

void ConfigTool::Quit()
  {
   // if (Datanames != NULL) delete Datanames;
   // if (Data != NULL) delete Data;
    filename="";Count=0;Datanames=NULL;Data=NULL;
  }

int ConfigTool::ReadFile()
  {
   char S[256];
   int ch;
   ifstream f;
   int Zeilen;
   f.open(filename.c_str(),ios::in);
   if (f.fail()) return 1;
   Zeilen=0;
   while (!f.eof())
		{	
            ch=f.get();
            if (ch== '\n')Zeilen++;
		}
   f.close();
   f.clear();
   Count = Zeilen+2;
   
   Datanames =new string [Count];
   Data=new string[Count];
   
   f.open(filename.c_str(),ios::in);
   if (f.fail()) 
                  { cout << "ERROR" << endl; return 2;}
   for (Zeilen=0;Zeilen!=Count;Zeilen++)
		{	
            f.getline(S,256,'='); 
            Datanames[Zeilen]=S;
            Data[Zeilen]="";
            while (true)
              {
               ch=f.get();
               if (ch=='\n') break;
               S[0]=ch;
               S[1]=0;
               Data[Zeilen]=Data[Zeilen]+S;
               if (ch==EOF) break;
              }
            ;
            
		}
   f.close();
   return 0;
  }
  
int ConfigTool::WriteFile()
  {
    ofstream f;
    string S;
    int i;
    f.open(filename.c_str(),ios::out);
    for (i=0;i!=Count;i++)
      {
         if (Datanames[i] != "")
           {
            S=Datanames[i]+"="+Data[i];
            f << S << endl;
           }
      }
    f.flush();
    f.close();
    return 0;
  }
  
bool ConfigTool::ParseName(int &Number,std::string Name)
  {
    int i;
    i=0;
    if (Count==0) return false;
    while (true)
      {
          if (Name==Datanames[i])
            {
              Number=i;
              return true;
            }
          i++;
          if (i==Count) break;
      }
    return false;
  }

 bool ConfigTool::Get(std::string Name, std::string &Val)
   {
      int i;
      if (ParseName(i,Name)==false) return false;
      Val = Data[i];
      return true;
   }
   
 bool ConfigTool::Get(std::string Name, int &Val)
   {
      int i;
      if (ParseName(i,Name)==false) 
	     { 
		  Val=0;
	     return false;
		 }
      Val = atol(Data[i].c_str());
      return true;
   }
 
 bool ConfigTool::Get(std::string Name, float &Val)
   {
     int i;
      if (ParseName(i,Name)==false) return false;
     Val = (float)atof(Data[i].c_str());
     return true;
   }
 
 bool ConfigTool::Get(std::string Name, double &Val)
   {
      int i;
      if (ParseName(i,Name)==false) return false;
      Val = atof(Data[i].c_str());
      return true;
   }
 bool ConfigTool::Get(std::string Name, bool &Val)
   {
     int _Val;                             
     bool ret;
     _Val = int(Val);
     ret = Get(Name,_Val);
     Val=(bool)_Val;
     return ret;
   }
   
/* bool ConfigTool::Get(std::string Name, void &(*DATA))
   {
     unsigned int i;
     if (ParseName(i,Name)==false) return false;
      
     // not ready yet 
     return true;
   }*/

 void ConfigTool::Add(const char *Name,const char *Val)
   {
      string *Datanames2,*Data2;
      int i;
      
      if (Count == 0)
        {
          Count++;
          Datanames=new string [Count];
          Data=new string[Count];
          Datanames[Count-1]=Name;
          Data[Count-1]=Val;
          return;
        }
      
      Count++;  
      Datanames2 =new string [Count];
      Data2=new string[Count];
      for (i=0;i!=Count-1;i++)
        {
          Datanames2[i]=Datanames[i];
          Data2[i]=Data[i];
        }
      delete Data;
      delete Datanames;
      Data=Data2;
      Datanames=Datanames2;
      Datanames[Count-1]=Name;
      Data[Count-1]=Val;
   }

void ConfigTool::Set(std::string Name, std::string Val)
  {
    Set(Name,Val.c_str());
  }

void ConfigTool::Set(std::string Name, const char *Val)
  {
    int i;
    if (Count==0) 
       {
         Add(Name.c_str(),Val);
         return;
       }
    if (ParseName(i,Name)==false)
       {
         Add(Name.c_str(),Val);
         return;
       };
    Data[i] = Val;
  } 
  
void ConfigTool::Set(std::string Name, int Val)
  {
    char S[128];
    sprintf(S,"%d",Val);
    Set(Name,S);
  }
void ConfigTool::Set(std::string Name, double Val)
  {
    char S[128];
    sprintf(S,"%f",Val);
    Set(Name,S);
  }
  
void ConfigTool::Set(std::string Name, bool Val)
  {
    Set(Name,(int)Val);
  }
  
void ConfigTool::Set(std::string Name, void *DATA, int _size)
  {
    char S[128];
    int i;
    int i2;
    string S2;
    sprintf(S,"%d",_size);
    S2=S;
    for (i=0;i!=_size;i++)
      {
         i2 = ((char*)DATA)[i];
         sprintf(S,"%d",i2);
         S2=S2+","+S;
      }
    Set(Name,S2.c_str());
  }
  
 unsigned long ConfigTool::GetCount()
   {
      return Count;
   }
