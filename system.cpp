
#include"system.h"
#include"globaldata.h"

#ifndef LWA_COLORKEY
#define LWA_COLORKEY	0x01
#define LWA_ALPHA	0x02
#define ULW_COLORKEY	0x01
#define ULW_ALPHA	0x02
#define ULW_OPAQUE	0x04
#endif

void SYSTEM::EnablePrivilege(LPSTR szPrivilege)
{
     HANDLE hToken;
     TOKEN_PRIVILEGES tkp;
     
     OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY|TOKEN_ADJUST_PRIVILEGES,&hToken);
     LookupPrivilegeValueA(NULL,szPrivilege,&tkp.Privileges[0].Luid);
     tkp.PrivilegeCount=1;
     tkp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
     AdjustTokenPrivileges(hToken,false,&tkp,0,NULL,0);
     CloseHandle(hToken);
}

void SYSTEM::LogOff()
{
     ExitWindowsEx(EWX_FORCE|EWX_LOGOFF,0);
}

void SYSTEM::Restart()
{
     EnablePrivilege(SE_SHUTDOWN_NAME);
     System.KillWindows(MainHandle);
     ExitWindowsEx(EWX_FORCE|EWX_REBOOT,0);
}

void SYSTEM::Shutdown()
{
     EnablePrivilege(SE_SHUTDOWN_NAME);
     System.KillWindows(MainHandle);
     ExitWindowsEx(EWX_FORCE|EWX_SHUTDOWN,0);
}

void SYSTEM::StandBy()
{
      EnablePrivilege(SE_SHUTDOWN_NAME);
      SetSystemPowerState(true,true);
}

bool CALLBACK SYSTEM::EnumKillWindows(HWND hwnd,LPARAM lParam)
{
     if(hwnd!=(HWND)lParam) System.KillProcess(hwnd);
}

void SYSTEM::KillProcess(HWND hwnd)
{
     DWORD   pid;
     HANDLE  hProcess;
     
     GetWindowThreadProcessId(hwnd,&pid);
     hProcess=OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);
     TerminateProcess(hProcess,0);
     CloseHandle(hProcess);
}

void SYSTEM::KillWindows(HWND hwnd)
{
     EnumWindows((WNDENUMPROC)EnumKillWindows,(LPARAM)hwnd);
}

void SYSTEM::SetTransparency(HWND hwnd,int percent)
{
     int result=255-((percent*255)/100);
     if(!(GetWindowLong(hwnd,GWL_EXSTYLE) & WS_EX_LAYERED))
       SetWindowLong(hwnd,GWL_EXSTYLE,GetWindowLong(hwnd,GWL_EXSTYLE)|WS_EX_LAYERED);
     
     if(!result)
      SetWindowLong(hwnd,GWL_EXSTYLE,GetWindowLong(hwnd,GWL_EXSTYLE)^WS_EX_LAYERED);
     
     typedef BOOL(__stdcall *HSLWA)(HWND,COLORREF,BYTE,DWORD);
     HMODULE hMod=LoadLibrary("user32.dll");
     HSLWA SetLayeredWindow=HSLWA(GetProcAddress(hMod,"SetLayeredWindowAttributes"));
     SetLayeredWindow(hwnd,0,(BYTE)result,LWA_ALPHA);
     FreeLibrary(hMod);
}
