
#include"button.h"

HWND BUTTON::CreateImageButton(LPSTR lpszTitle,int x,int y,int cx,int cy,HWND hwnd,UINT idCommand,HIMAGELIST hImage,int left ,int top,int right,int bottom,int align)
{
     HWND hWnd;
     IMAGE_BUTTON img;
     
     hWnd=CreateWindowEx(0,"button",lpszTitle,WS_CHILD|WS_VISIBLE,x,y,cx,cy,hwnd,(HMENU)idCommand,NULL,NULL);
     
     img.hImageList=hImage;
     img.rc.left=left;
     img.rc.right=right;
     img.rc.top=top;
     img.rc.bottom=bottom;
     img.align=align;
     
     SendMessage(hWnd,BCM_SETIMAGELIST,0,(LPARAM)&img);
     
     return hWnd;
}
