
#ifndef REGISTRY_H
#define REGISTRY_H

#include<windows.h>

class REGISTRY
{
      public:
             void WriteNumber(HKEY,LPSTR,LPSTR,int);
             void WriteString(HKEY,LPSTR,LPSTR,LPSTR);
             void WriteNumberSetting(LPSTR,LPSTR,int);
             void WriteStringSetting(LPSTR,LPSTR,LPSTR);
             
             int  ReadNumber(HKEY,LPSTR,LPSTR);
             char *ReadString(HKEY,LPSTR,LPSTR);
             char *ReadStringSetting(LPSTR,LPSTR);
             int  ReadNumberSetting(LPSTR,LPSTR);
};

#endif
