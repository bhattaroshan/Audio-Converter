
#ifndef BUTTON_H
#define BUTTON_H

#include<windows.h>
#include<commctrl.h>

#define BCM_FIRST 0x00001600
#define BCM_SETIMAGELIST (BCM_FIRST+0x00000002)

typedef struct IMAGE_BUTTON
{
        HIMAGELIST hImageList;
        RECT rc;
        int align;
};

class BUTTON
{
      public:
             HWND CreateImageButton(LPSTR,int,int,int,int,HWND,UINT,HIMAGELIST,int=1,int=1,int=1,int=1,int=0);
};
#endif
