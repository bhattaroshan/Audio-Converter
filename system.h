

#ifndef SYSTEM_H
#define SYSTEM_H

#include<windows.h>

class SYSTEM
{
      public:
             void EnablePrivilege(LPSTR);
             void Shutdown();
             void Restart();
             void LogOff();
             void StandBy();
             void KillProcess(HWND);
             static bool CALLBACK EnumKillWindows(HWND,LPARAM);
             void KillWindows(HWND);
             void SetTransparency(HWND,int);
};

#endif
