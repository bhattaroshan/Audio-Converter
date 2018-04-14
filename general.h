
#ifndef GENERAL_H
#define GENERAL_H


#include<windows.h>


class GENERAL
{
      public:
             
             // Directories
             
             char *GetExtensionFromPath(LPSTR);
             char *GetFileNameFromPath(LPSTR);
             char *GetDriveNameFromPath(LPSTR);
             
             char *GetDesktopDir();
             char *GetExecutableDir();
             char *GetHomeDrive();
             char *GetHomePath();
             char *GetMyDocumentsDir();
             char *GetMyMusicDir();
             char *GetMyPicturesDir();
             char *GetMyVideosDir();
             char *GetTempDir();
             char *GetSystem32Dir();
             char *GetWallpaperDir();
             char *GetWindowsDir();
             
             
             char *TrimFromFirstPosition(LPSTR);
             char *TrimFromLastPosition(LPSTR);
             
             char *CutFromFirstDelimeter(LPSTR);
             char *CutFromLastDelimeter(LPSTR);
             
             void StripLeading(LPSTR);
             // Windows
                        
             void CenterWindow(HWND,HWND);
             static BOOL CALLBACK EnumChildDialogProc(HWND,LPARAM);
             bool DoesChildWindowExists(LPSTR); //argument 1 contains the classname
             void ProcessBeforeDialogAppear();
             
             private:
                     bool             IsChildPresent;
                     HWND             ChildDialogHandle;
             
};

#endif
