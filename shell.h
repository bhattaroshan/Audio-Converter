
#ifndef SHELL_H
#define SHELL_H

#include<windows.h>
#include<shlobj.h>

class SHELL
{
      public:
             int SHProperties(LPSTR);
             char*FileSelectDialog(HWND,LPSTR);
             char*BrowseForFolder(HWND);
};

#endif
