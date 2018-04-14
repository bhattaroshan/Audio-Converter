/****************************************************************************
 *                                                                          *
 * File    : odmenu.c                                                       *
 *                                                                          *
 * Purpose : Owner-draw menu functions.                                     *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <tchar.h>
#include <stdlib.h>
#include "odmenu.h"
//#include "main.h"
// #include "resource.h"

ODMENU::ODMENU()
{     
   
   index=0;
   
   GetColor[0]=GetColor[1]=GetColor[2]=GetColor[3]=false;
   
   hImage=ImageList_Create(16,16,0x0001|0x0020,0,1);
   
   
   nMenuBkClr= RGB(255,255,255);
   nMenuIconBkClr=RGB(219,216,216);
   nMenuSelectBkClr=RGB(210,189,182);
   nMenuSelectRectClr=RGB(133,66,64);
   nMenuTextClr=RGB(0,0,0);
   nMenuSelectTextClr=RGB(0,0,0);
   MenuHandleCount=0;
   MenuHandle=(HMENU*)malloc(1*sizeof(HMENU));
   
}


ODMENU::~ODMENU()
{
    ImageList_Destroy(hImage);
    free(MenuHandle);
}


void ODMENU::SetDefaultMenu()
{
   nMenuBkClr= RGB(255,255,255);
   nMenuIconBkClr=RGB(219,216,216);
   nMenuSelectBkClr=RGB(210,189,182);
   nMenuSelectRectClr=RGB(133,66,64);
   nMenuTextClr=RGB(0,0,0);
   nMenuSelectTextClr=RGB(0,0,0);
   
   GetColor[0]=true;
   GetColor[1]=GetColor[2]=GetColor[3]=false;
}

void ODMENU::SetRedMenu()
{
   nMenuBkClr= RGB(255,221,170);
   nMenuIconBkClr=RGB(187,102,85);
   nMenuSelectBkClr=RGB(192,160,112);
   nMenuSelectRectClr=RGB(64,66,133);
   nMenuTextClr=RGB(0,0,0);
   nMenuSelectTextClr=RGB(0,0,0);
   
   GetColor[1]=true;
   GetColor[0]=GetColor[2]=GetColor[3]=false;
}

void ODMENU::SetGreenMenu()
{
     nMenuBkClr= RGB(170,255,170);
     nMenuIconBkClr=RGB(106,187,106);
     nMenuSelectBkClr=RGB(136,204,187);
     nMenuSelectRectClr=RGB(119,34,34);
     nMenuTextClr=RGB(0,0,0);
     nMenuSelectTextClr=RGB(0,0,119);
     
   GetColor[2]=true;
   GetColor[0]=GetColor[1]=GetColor[3]=false;
}

void ODMENU::SetBlueMenu()
{
     nMenuBkClr= RGB(184,184,255);
     nMenuIconBkClr=RGB(119,136,187);
     nMenuSelectBkClr=RGB(34,34,102);
     nMenuSelectRectClr=RGB(170,119,68);
     nMenuSelectTextClr=RGB(255,255,102);
     nMenuTextClr=RGB(0,0,0);
     
   GetColor[3]=true;
   GetColor[0]=GetColor[1]=GetColor[2]=false;
}

void ODMENU::SetMenuColor()
{
     if(GetColor[0]) SetDefaultMenu();
     if(GetColor[1]) SetRedMenu();
     if(GetColor[2]) SetGreenMenu();
     if(GetColor[3]) SetBlueMenu();
}

/****************************************************************************
 *                                                                          *
 * Function: ODMenu_OnMeasureItem                                           *
 *                                                                          *
 * Purpose : Handle WM_MEASUREITEM message.                                 *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

void ODMENU::ODMenu_OnMeasureItem(HWND hwnd, MEASUREITEMSTRUCT *pmis)
{
    PMYMENUITEM pmmi;

    pmmi = (PMYMENUITEM)pmis->itemData;

    if (pmis->CtlType != ODT_MENU || pmmi == NULL)
    {
        // The message is not for us, pass it on.
        //FORWARD_WM_MEASUREITEM(hwnd, pmis, ODMenu_DefProc);
        return;
    }

    if (pmmi->fType & MFT_SEPARATOR)
    {
        pmis->itemHeight = CYMENUITEM / 2;
        pmis->itemWidth = CXYTOOLIMAGE * 2;
    }
    else
    {
        RECT rcItem = {0};
        int cxItem;
        int cyItem;
        HDC hdc;

        hdc = GetDC(NULL);
        if (hdc)
        {
         NONCLIENTMETRICS ncm;
         HFONT g_hFont;

            ncm.cbSize = sizeof(ncm);
            if (SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0))
                g_hFont = CreateFontIndirect(&ncm.lfMenuFont);
        
            HFONT hOldFont=SelectFont(hdc, g_hFont);
            DrawText(hdc, pmmi->szText, -1, &rcItem, DT_CALCRECT|DT_LEFT|DT_SINGLELINE);
            ReleaseDC(NULL, hdc);
            SelectObject(hdc,hOldFont);
            DeleteObject(g_hFont);
        }

        cyItem = (rcItem.bottom - rcItem.top);
        cyItem = max(cyItem, CYMENUITEM) + CYTOOLBORDER * 2;

        cxItem = (rcItem.right - rcItem.left);
        cxItem += CXYTOOLIMAGE * 2 + CXTOOLBORDER * 2 + CXTEXTMARGIN+30;

        pmis->itemHeight = cyItem;
        pmis->itemWidth = cxItem;
    }
    index=0;
}

/****************************************************************************
 *                                                                          *
 * Function: Main_OnDrawItem                                                *
 *                                                                          *
 * Purpose : Handle WM_DRAWITEM message.                                    *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

 void ODMENU::ODMenu_OnDrawItem(HWND hwnd, const DRAWITEMSTRUCT *pdis)
{
   if (pdis->CtlType == ODT_MENU)
   {   
     
      PMYMENUITEM pmmi;
     
      pmmi = (PMYMENUITEM)pdis->itemData;
     
      RECT rcTool,rcItem;
      rcTool.top = rcItem.top = pdis->rcItem.top;
      rcTool.bottom = rcItem.bottom = pdis->rcItem.bottom;
      rcTool.left = pdis->rcItem.left;
      rcTool.right = rcTool.left + CXYTOOLIMAGE + CXTOOLBORDER * 2;
      rcItem.left = rcTool.right+5;
      rcItem.right = pdis->rcItem.right -10;
     
     
     
      RECT   stItemRect;
      SetRect(&stItemRect,pdis->rcItem.left,pdis->rcItem.top,pdis->rcItem.right,pdis->rcItem.bottom);
     
      int   nIconSize       = 16;
	  int   nCheckX         = 16;
      int    nSpace		    = 2;
     
      HBRUSH hBrush	        = NULL;
	  HBRUSH hOldBrush	    = NULL;
	  COLORREF nClrSel	    = 0;
	  COLORREF nClrTxt		= 0;
	  HBRUSH hBorderBrush	= NULL;
	  int  nLen			    = 0;
	  UINT stSize		    = 0;
   
      BOOL fDisabled = pdis->itemState & ODS_GRAYED;
      BOOL fSelected = pdis->itemState & ODS_SELECTED;
      BOOL fChecked = pdis->itemState & ODS_CHECKED;
      
       if(fSelected)
        {
          hBorderBrush	= CreateSolidBrush(nMenuSelectRectClr);  //bound rectangle color
          
          if(fDisabled)
           {
             hBrush	 = GetSysColorBrush(14);
             nClrSel = GetSysColor(14);
           }
          else
           {
             hBrush		= CreateSolidBrush(nMenuSelectBkClr); //menu select color
			 nClrSel	= nMenuSelectBkClr;
           } 
        }
       else
        {
            hBrush	= CreateSolidBrush(nMenuBkClr);  //Menu Bk Color
			nClrSel	= nMenuBkClr;                    //Icon BK color for check box
        }
        
       if(fSelected && !fDisabled)
       {
         nClrTxt	= SetTextColor(pdis->hDC, GetSysColor(14));
         
       }
       else if(fDisabled)
        {
           nClrTxt	= SetTextColor(pdis->hDC, GetSysColor(17));
        }
       else
        {
           nClrTxt	= SetTextColor(pdis->hDC, GetSysColor(7));
        } 
        
        COLORREF nClrBk	= SetBkColor(pdis->hDC, nClrSel);
        hOldBrush		= (HBRUSH)SelectObject(pdis->hDC,  hBrush);
        
        FillRect(pdis->hDC, &stItemRect, hBrush);
        SelectObject(pdis->hDC, hOldBrush);
		DeleteObject(hBrush);
		
		if(!fSelected || fDisabled)
		 {
           SetRect(&stItemRect, pdis->rcItem.left, pdis->rcItem.top, pdis->rcItem.left + 2 * nSpace + nIconSize + 1, pdis->rcItem.bottom);
           
             hBrush	= CreateSolidBrush(nMenuIconBkClr);        //ICON BK COLOR
			 hOldBrush	= (HBRUSH)SelectObject(pdis->hDC,hBrush);
    		 FillRect(pdis->hDC, &stItemRect, hBrush); 
					
			 SelectObject(pdis->hDC, hOldBrush);
			 DeleteObject(hBrush);
         }
         
        if(fChecked)
         {
          SetRect(&stItemRect, pdis->rcItem.left+ 1, pdis->rcItem.top + 1, pdis->rcItem.left + nIconSize + nSpace + 1, pdis->rcItem.top + nIconSize + nSpace + 1);
          
          if(fSelected)
           {
             hBrush	= GetSysColorBrush(13);
             SetTextColor(pdis->hDC,nMenuSelectTextClr);
           }
          else
           {
             hBrush	= GetSysColorBrush(4);
             SetTextColor(pdis->hDC,nMenuTextClr);
           } 
           
            hOldBrush	= (HBRUSH)SelectObject(pdis->hDC, hBrush);
			FillRect(pdis->hDC, &stItemRect, hBrush);
			SelectObject(pdis->hDC, hOldBrush);
			DeleteObject(hBrush);
			
			HDC hDCBitmap	    = CreateCompatibleDC(pdis->hDC);
			HBITMAP hbmpCheck	= CreateBitmap(nIconSize, nIconSize, 1, 1, 0);
			HBITMAP hbmpOld		= (HBITMAP)SelectObject(hDCBitmap, hbmpCheck);
			
			int x = stItemRect.left + (nIconSize + nSpace - nCheckX + 2) / 2;
			int y = stItemRect.top + (nIconSize + nSpace - nCheckX + 2) / 2 - nSpace;
			
			SetRect(&stItemRect, 0, 0, nIconSize, nIconSize);
			
			DWORD nCtrlStyle = 0x0001;
			
			if(IsMenuRadio(pdis->itemID)) nCtrlStyle=0x0002;
			
			DrawFrameControl(hDCBitmap, &stItemRect, 2, nCtrlStyle);
			
			BitBlt(pdis->hDC, x, y + 1, nCheckX, nCheckX, hDCBitmap, 0, 0, 0x00CC0020);
			
			SetRect(&stItemRect, pdis->rcItem.left + 1, pdis->rcItem.top + 1, pdis->rcItem.left + nIconSize + nSpace + 1, pdis->rcItem.top + nIconSize + nSpace + 1);
         
            hBrush	= CreateSolidBrush(nMenuSelectRectClr);
			hOldBrush	= (HBRUSH)SelectObject(pdis->hDC, hBrush);
			FrameRect(pdis->hDC, &stItemRect, hBrush);
			SelectObject(pdis->hDC, hOldBrush);
			DeleteObject(hBrush);
			
			SelectObject(hDCBitmap, hbmpOld);
			DeleteObject(hbmpCheck);
			DeleteDC(hDCBitmap);
         } 
       SetRect(&stItemRect, pdis->rcItem.left, pdis->rcItem.top, pdis->rcItem.right, pdis->rcItem.bottom);
       
       if(fSelected)
        {
          hOldBrush	= (HBRUSH)SelectObject(pdis->hDC, hBorderBrush);
		  FrameRect(pdis->hDC, &stItemRect, hBorderBrush);
		  SelectObject(pdis->hDC, hOldBrush);
		  DeleteObject(hBorderBrush);
        }
        
        int nSaveLeft	= stItemRect.left;
        int nLeft		= nSaveLeft;
        nLeft += nSpace;
		nLeft += nSpace;
		nLeft += nIconSize;
		nLeft += nSpace + 2;
		
		stItemRect.left=nLeft;
        
        if(fDisabled)
           DrawMenuText(pdis->hDC, pmmi->szText, &rcItem,GetSysColor(COLOR_GRAYTEXT));
           
        else if(fSelected)
           DrawMenuText(pdis->hDC, pmmi->szText, &rcItem,nMenuSelectTextClr);
        
        else
           DrawMenuText(pdis->hDC, pmmi->szText, &rcItem,nMenuTextClr);
       
       
       if(!fChecked && ! (pmmi->fType & MFT_SEPARATOR))
        {
         if(fDisabled)
          {
          ImageList_DrawEx(hImage,pmmi->index,pdis->hDC,nSpace,stItemRect.top+2,0,0,0xFFFFFFFF,0xFFFFFFFF,0x0001 | 0x0004);
          }
         else
          {
          ImageList_Draw(hImage,pmmi->index,pdis->hDC,nSpace,stItemRect.top+2,0x0001);
          }
        }
        
       if (pmmi->fType & MFT_SEPARATOR)
       {
        
        stItemRect.left=nSaveLeft;
        stItemRect.left=stItemRect.left;
        stItemRect.top=stItemRect.top + 1;
		stItemRect.bottom=stItemRect.left+2;
        DrawEdge(pdis->hDC, &stItemRect, 0x0006, 0x0002);
        
       }
       
       
       SetTextColor(pdis->hDC, nClrTxt);
	   SetBkColor(pdis->hDC, nClrBk);
   }
   
}

/****************************************************************************
 *                                                                          *
 * Function: DrawMenuText                                                   *
 *                                                                          *
 * Purpose : Draw the text for a menu item.                                 *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

void ODMENU::DrawMenuText(HDC hdc, LPSTR pszText, PRECT prcItem, COLORREF clrText)
{
    LPSTR pszTab;
    int bkMode;

    // Make sure we can restore the DC when we're done.
    clrText = SetTextColor(hdc, clrText);
    bkMode = SetBkMode(hdc, TRANSPARENT);
    pszTab = _tcschr(pszText, '\t');
    if (pszTab != NULL)
    {
        // Draw the menu and accelerator texts.
        DrawText(hdc, pszText, (pszTab - pszText), prcItem, DT_MYSTYLE);
        DrawText(hdc, pszTab+1, -1, prcItem, DT_MYSTYLE|DT_RIGHT);
    }
    else
    {
        // Draw the menu text.
        DrawText(hdc, pszText, -1, prcItem, DT_MYSTYLE);
    }

    SetTextColor(hdc, clrText);
    SetBkMode(hdc, bkMode);
    
}

/****************************************************************************
 *                                                                          *
 * Function: ConvertMenu                                                    *
 *                                                                          *
 * Purpose : Convert menu items to/from owner-draw style.                   *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

void ODMENU::ConvertMenu(HMENU hMenu, BOOL fOwnerDraw)
{
    HIMAGELIST himl = NULL;
    int cItems;
    int iItem;
    bool istrue=false;
    int list=0;
    
    cItems = GetMenuItemCount(hMenu);
    
    for(int i=0;i<MenuHandleCount;i++)
     {
            if(MenuHandle[MenuHandleCount]==hMenu) istrue=true;
     }
    
    if(!istrue)
    {
    MenuHandle[MenuHandleCount]=hMenu;
    MenuHandleCount++;
    MenuHandle=(HMENU*)realloc(MenuHandle,(MenuHandleCount+1)*sizeof(HMENU));
    }
    
    for (iItem = 0; iItem < cItems; iItem++)
    {
        TCHAR szText[CCHMAXMENUTEXT];
        MENUITEMINFO mii;
        PMYMENUITEM pmmi;

        mii.cbSize = sizeof(mii);
        mii.fMask = MIIM_SUBMENU|MIIM_DATA|MIIM_ID|MIIM_TYPE;
        mii.dwTypeData = szText;
        mii.cch = NELEMS(szText);
        if (!GetMenuItemInfo(hMenu, iItem, TRUE, &mii))
            continue;

        if (mii.fType & MFT_BITMAP)
            continue;

        pmmi = (PMYMENUITEM)mii.dwItemData;
        
        
        mii.fMask = 0;

        if (fOwnerDraw)
        {
            if (!(mii.fType & MFT_OWNERDRAW))
            {
                mii.fType |= MFT_OWNERDRAW;
                mii.fMask |= MIIM_TYPE;

                if (!pmmi)
                {
                    pmmi =(_MYMENUITEM*) malloc(sizeof(*pmmi));
                    
                    pmmi->index=list++;
                    
                    if (pmmi)
                    {
                        pmmi->fType = mii.fType;
                        pmmi->hIcon = NULL;
                        mii.dwItemData = (DWORD)pmmi;
                        mii.fMask |= MIIM_DATA;
                    }
                }

                if (!(mii.fType & MFT_SEPARATOR))  /* MFT_STRING is zero! */
                    lstrcpyn(pmmi->szText, szText, NELEMS(pmmi->szText));
                else
                    pmmi->szText[0] = '\0';
                
                
                   
                pmmi->index=index++;
            }
 
        }
        else
        {
            if (pmmi != NULL)
            {
                if (mii.fType & MFT_OWNERDRAW)
                {
                    mii.fType &= ~MFT_OWNERDRAW;
                    mii.fMask |= MIIM_TYPE;

                    lstrcpyn(szText, pmmi->szText, NELEMS(szText));
                    mii.dwTypeData = szText;
                }

                mii.dwItemData = 0;
                mii.fMask |= MIIM_DATA;

                if (pmmi->hIcon != NULL)
                    DestroyIcon(pmmi->hIcon);

                free(pmmi);
                pmmi = NULL;
            }
        }

        if (mii.fMask)
        {
            SetMenuItemInfo(hMenu, iItem, TRUE, &mii);
        }
    }
}


bool ODMENU::IsMenuRadio(UINT menuid)
{
     int i,j;
     
     for(i=0;i<MenuHandleCount;i++)
     {
      for(j=0;j<GetMenuItemCount(MenuHandle[i]);j++)
      {
        UINT id=GetMenuItemID(MenuHandle[i],j);
        
         if(menuid==id)
         {
           MENUITEMINFO mii;

           mii.cbSize = sizeof(mii);
           mii.fMask = MIIM_SUBMENU|MIIM_DATA|MIIM_ID|MIIM_TYPE;
           mii.wID=id;
           GetMenuItemInfo(MenuHandle[i], j, TRUE, &mii);
           
           if(mii.fType & MFT_RADIOCHECK)
             return true;
           else
             return false;
         }
      }
     }
}

void ODMENU::AddMenuIcon(HICON hIcon,int index)
{
     if(index<=0)
        {
         ImageList_AddIcon(hImage,hIcon);
        }
     else
        {
         HICON icon=ExtractIcon(NULL,"%systemroot%\\system32\\shell32.dll",index);
         ImageList_AddIcon(hImage,icon);
        }
}

void ODMENU::RecreateImage()
{
     ImageList_Destroy(hImage);
     hImage=ImageList_Create(16,16,0x0001|0x0020,0,1);
}

void ODMENU::DestroyImage()
{
     ImageList_Destroy(hImage);
}
/*
    return;
    }

    rcTool.top = rcItem.top = pdis->rcItem.top;
    rcTool.bottom = rcItem.bottom = pdis->rcItem.bottom;
    rcTool.left = pdis->rcItem.left;
    rcTool.right = rcTool.left + CXYTOOLIMAGE + CXTOOLBORDER * 2;
    rcItem.left = rcTool.right;
    rcItem.right = pdis->rcItem.right;

    if (pmmi->fType & MFT_SEPARATOR)
    {
        FillRect(pdis->hDC, &rcTool, GetSysColorBrush(COLOR_BTNFACE));
        FillRect(pdis->hDC, &rcItem, GetSysColorBrush(COLOR_MENU));
        rcItem.top += (rcItem.bottom - rcItem.top) / 2;
        rcItem.left += CXTEXTMARGIN;
        DrawEdge(pdis->hDC, &rcItem, EDGE_ETCHED, BF_TOP);
    }
    else
    {
        BOOL fDisabled = pdis->itemState & ODS_GRAYED;
        BOOL fSelected = pdis->itemState & ODS_SELECTED;
        BOOL fChecked = pdis->itemState & ODS_CHECKED;

        // Draw the tool background.
        FillRect(pdis->hDC, &rcTool, GetSysColorBrush(fSelected && !fDisabled ? COLOR_HIGHLIGHT : COLOR_BTNFACE));

        // Do we have an image to display?
        if (fChecked)
        {

            if (fChecked && !fSelected)
             {

             DrawEdge(pdis->hDC, &rcTool, BDR_SUNKENOUTER, BF_RECT);
            // InflateRect(&rcTool, -CXTOOLBORDER, -CYTOOLBORDER);
              rcTool.right-=2;
              InflateRect(&rcTool, -1, -1);
              
             FrameRect(pdis->hDC, &rcTool, GetStockBrush(BLACK_BRUSH));
             FillRect(pdis->hDC, &rcTool, GetStockBrush(COLOR_HIGHLIGHT));
             //DrawFrameControl(pdis->hDC,&rcTool,DFCS_BUTTONCHECK,DFCS_CHECKED);
             }
            
            
        }
        
        
        FillRect(pdis->hDC, &rcItem, GetSysColorBrush(fSelected && !fDisabled ? COLOR_HIGHLIGHT : COLOR_MENU));

        rcItem.left += CXTEXTMARGIN;
        rcItem.right -= CXYTOOLIMAGE;

        if (fDisabled && !fSelected)
        {
            // Draw the menu item text shadow.
            OffsetRect(&rcItem, +1, +1);
            DrawMenuText(pdis->hDC, pmmi->szText, &rcItem, GetSysColor(COLOR_3DHILIGHT));
            OffsetRect(&rcItem, -1, -1);
        }

        // Draw the menu item text.
        DrawMenuText(pdis->hDC, pmmi->szText, &rcItem, GetSysColor(
            fDisabled ? COLOR_GRAYTEXT : fSelected ? COLOR_HIGHLIGHTTEXT : COLOR_MENUTEXT));

        if (fSelected)
            FrameRect(pdis->hDC, &pdis->rcItem, GetStockBrush(BLACK_BRUSH));
    }
*/
