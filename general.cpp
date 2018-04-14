
#include"general.h"
#include"globaldata.h"


///////////////////////////////////////////////////////////////////
//                                                               // 
//                         DIRECTORIES                           //  
//                                                               //
///////////////////////////////////////////////////////////////////


char* GENERAL::GetDesktopDir()
{
     return Registry.ReadString(HKEY_CURRENT_USER, 
                          "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", 
                           "Desktop");
}

char* GENERAL::GetHomeDrive()
{
    static char szValue[MAX_PATH]="\0";
    GetEnvironmentVariable("HOMEDRIVE",szValue,MAX_PATH);
    return szValue;
}

char* GENERAL::GetHomePath()
{
    static char szValue[MAX_PATH]="\0";
    char temp[MAX_PATH]="\0";
    GetEnvironmentVariable("HOMEPATH",temp,MAX_PATH);
    strcpy(szValue,GetHomeDrive());
    strcat(szValue,temp);
    return szValue;
}

char* GENERAL::GetMyDocumentsDir()
{
     return Registry.ReadString(HKEY_CURRENT_USER, 
                          "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", 
                           "Personal");
}

char* GENERAL::GetMyMusicDir()
{
     return Registry.ReadString(HKEY_CURRENT_USER, 
                          "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", 
                           "My Music");
}

char* GENERAL::GetMyPicturesDir()
{
     return Registry.ReadString(HKEY_CURRENT_USER, 
                          "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", 
                           "My Pictures");
}

char* GENERAL::GetMyVideosDir()
{
     return Registry.ReadString(HKEY_CURRENT_USER, 
                          "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", 
                           "My Video");
}

char* GENERAL::GetTempDir()
{
    static char szValue[MAX_PATH]="\0";  
    GetTempPath(MAX_PATH, szValue);
    
    return szValue;
}

char* GENERAL::GetSystem32Dir()
{
        static char szValue[MAX_PATH]="\0";  
      	GetSystemDirectory(szValue,MAX_PATH);
      	
      	return szValue;
}

char* GENERAL::GetWallpaperDir()
{
      static char szValue[MAX_PATH]="\0";
      strcpy(szValue,Registry.ReadString(HKEY_CURRENT_USER, 
                          "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", 
                           "AppData"));
      strcat(szValue,"\\Microsoft");
      
      return szValue;
}

char* GENERAL::GetWindowsDir()
{
        static char szValue[MAX_PATH]="\0";  
      	GetWindowsDirectory(szValue,MAX_PATH);
      	
      	return szValue;
}


char* GENERAL::GetExecutableDir()
{
      int i;
      static char szValue[MAX_PATH]="\0";
      GetModuleFileName(NULL,szValue,sizeof(szValue)+1);
      
      for(i=strlen(szValue)-1;i>=0;i--)
        if(szValue[i]=='\\') break; 
      
      szValue[i]='\0';
      
      return szValue;
}

void GENERAL::StripLeading(LPSTR szText)
{
     int i=0;
     int j=0;
     
     while(szText[i]!='\0')
     {
       if(szText[i]==' ') i++;
       else break;
     }
     
     while(szText[i]!='\0')
      szText[j++]=szText[i++];
      
     szText[j]='\0';
}

///////////////////////////////////////////////////////////////////
//                                                               // 
//                          WINDOWS                              //  
//                                                               //
///////////////////////////////////////////////////////////////////


void GENERAL::CenterWindow(HWND nParent,HWND nChild)
{
     int parent_cx,parent_cy;
     int child_cx,child_cy;
     int x,y;
     RECT rcParent;
     RECT rcChild;
     
     if(nParent==NULL)
         nParent=GetDesktopWindow();
         
     GetWindowRect(nParent,&rcParent);
     GetWindowRect(nChild,&rcChild);
     
     parent_cx=rcParent.right-rcParent.left;
     parent_cy=rcParent.bottom-rcParent.top;
     
     child_cx=rcChild.right-rcChild.left;
     child_cy=rcChild.bottom-rcChild.top;
     
     x=rcParent.left+((parent_cx/2)-(child_cx/2));
     y=rcParent.top+((parent_cy/2)-(child_cy/2));
     
     SetWindowPos(nChild,NULL,x,y,0,0,SWP_NOSIZE|SWP_NOZORDER);
}

BOOL CALLBACK GENERAL::EnumChildDialogProc(HWND hwnd,LPARAM lParam)
{
     char szClassName[255];
     char szWindowName[255];
     
     GetClassName(hwnd,szClassName,sizeof(szClassName)+1);
     GetWindowText(hwnd,szWindowName,sizeof(szWindowName)+1);
     
     if(!strcmp(szClassName,(char*)lParam))
       {
         General.IsChildPresent=true;
         General.ChildDialogHandle=hwnd;
         return false;
       }
       
     if(!strcmp(szWindowName,(char*)lParam))
       {
         General.IsChildPresent=true;
         General.ChildDialogHandle=hwnd;
         return false;
       }
return true;
}

bool GENERAL::DoesChildWindowExists(LPSTR szClassName)
{
    General.IsChildPresent=false;
    General.ChildDialogHandle=NULL;
    EnumThreadWindows(GetCurrentThreadId(),EnumChildDialogProc,(LPARAM)szClassName);
    if(General.IsChildPresent) 
       return true;
    else 
       return false;
}

void GENERAL::ProcessBeforeDialogAppear()
{
    if(IsIconic(General.ChildDialogHandle))
        ShowWindow(General.ChildDialogHandle,SW_RESTORE);
    
    Sleep(10); //Wait For Window To come Normal
    
    SetForegroundWindow(General.ChildDialogHandle);
}

