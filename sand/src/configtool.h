#ifndef __CONFIGTOOL_H__
#define  __CONFIGTOOL_H__

#include <string>

// by Christian Klein
// 15.Dec 2005


// 12.Feb. 2006: there seems to be an error with the adding of variables
// 				 only sometimes (don't know where it comes from (maybe a memory problem)

class ConfigTool
{
 public:
        ConfigTool(){filename="";Count=0;Datanames=NULL;Data=NULL;};
        ~ConfigTool(){Quit();};
 
 void   Init(std::string _filename);
 void   GetFileName(std::string &_filename);
 void   Quit();
 int    ReadFile();
 int    WriteFile();
 
 bool   Get(std::string Name, std::string &Val);
 bool   Get(std::string Name, int &Val);
 bool   Get(std::string Name, float &Val);
 bool   Get(std::string Name, double &Val);
 bool   Get(std::string Name, bool &Val);
// bool   Get(std::string Name, void &(*DATA));  // not ready yet
 
 void   Set(std::string Name, std::string Val);
 void   Set(std::string Name, const char *Val);
 void   Set(std::string Name, int Val);
 void   Set(std::string Name, double Val);
 void   Set(std::string Name, bool Val);
 void   Set(std::string Name, void *DATA, int _size);
 
 
 unsigned long GetCount();
 
 protected:
 bool   ParseName(int &Number,std::string Name);
 void   Add(const char *Name,const char *Val);
 
        std::string filename;
        std::string *Datanames,*Data;
        int Count;
};


#endif
