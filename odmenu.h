// odmenu.h

#ifndef _ODMENU_H
#define _ODMENU_H

extern "C"
{

#define NELEMS(a)  (sizeof(a) / sizeof(a[0]))

#define DT_MYSTYLE  (DT_LEFT|DT_VCENTER|DT_SINGLELINE)

#define CCHMAXMENUTEXT  128
#define CXTEXTMARGIN    6
#define CXTOOLBORDER    4
#define CYTOOLBORDER    2
#define CXYTOOLIMAGE    16
#define CYMENUITEM      16

#include<shellapi.h>

// Function prototypes.
typedef struct _MYMENUITEM *PMYMENUITEM;  // pmmi
typedef struct _MYMENUITEM  // mmi
{
    TCHAR szText[CCHMAXMENUTEXT];   // Saved item text.
    UINT  fType;                     // Saved item flags.
    HICON hIcon;                    // Toolbar image as an Icon.
    //bool  radio;
    //HICON icon1;
    //UINT  id;
    int   index;
} MYMENUITEM;


class ODMENU
{
      public:
             ODMENU();
             ~ODMENU();
             void ODMenu_OnMeasureItem(HWND, MEASUREITEMSTRUCT *);
             void ODMenu_OnDrawItem(HWND, const DRAWITEMSTRUCT *);
             void DrawMenuText(HDC, LPSTR, PRECT, COLORREF);
             void ConvertMenu(HMENU, BOOL);
             bool IsMenuRadio(UINT);
             void AddMenuIcon(HICON,int);
             void RecreateImage();
             void DestroyImage();
             
             void SetODMenuTextColor(COLORREF hBrush){nMenuTextClr=hBrush;}
             void SetODMenuSelectTextColor(COLORREF hBrush){nMenuSelectTextClr=hBrush;}
             void SetODMenuBkColor(COLORREF hBrush){nMenuBkClr=hBrush;}
             void SetMenuSelectBkColor(COLORREF hBrush){nMenuSelectBkClr=hBrush;}
             void SetODMenuIconBkColor(COLORREF hBrush){nMenuIconBkClr=hBrush;}
             void SetODMenuRectColor(COLORREF hBrush){nMenuSelectRectClr=hBrush;}
             
             
             void SetGreenMenu();
             void SetRedMenu();
             void SetBlueMenu();
             void SetDefaultMenu();
             void SetMenuColor();
             
      private:
             COLORREF    nMenuBkClr;
             COLORREF    nMenuIconBkClr;
             COLORREF    nMenuSelectBkClr;
             COLORREF    nMenuSelectRectClr;
             COLORREF    nMenuSelectTextClr;
             COLORREF    nMenuTextClr;
             bool        GetColor[4];
             
             HIMAGELIST  hImage;
             int         index;
             HMENU       *MenuHandle;
             int         MenuHandleCount;
};

#endif // _ODMENU_H
}
