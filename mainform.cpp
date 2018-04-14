#include<windows.h>
#include"globaldata.h"


int APIENTRY WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR nCmdShow,int nCmdLine)
{ 
    MSG msg;
    HWND hwnd;
    HACCEL hAccel;
    
    
    hwnd=AudioConverter.AudioConverterDialog();
    MainHandle=hwnd;
    
    ShowWindow(hwnd,nCmdLine);
     
    hAccel=LoadAcceleratorsA(NULL,MAKEINTRESOURCE(IDA_AUDIO_CONVERTER));
    
    while(GetMessage(&msg,NULL,0,0))
    {
       if(!TranslateAcceleratorA(hwnd,hAccel,&msg))
        {
         TranslateMessage(&msg);
         DispatchMessage(&msg);
        }
       
    }
return msg.wParam;                                                                               
}


void open()
{
     GetOpenFileName(NULL);
}
