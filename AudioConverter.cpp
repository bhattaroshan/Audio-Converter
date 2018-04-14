
#include"AudioConverter.h"
#include"globaldata.h"

#ifndef LVM_APPROXIMATEVIEWRECT
#define LVM_APPROXIMATEVIEWRECT (LVM_FIRST+64)
#define LVM_SETEXTENDEDLISTVIEWSTYLE (LVM_FIRST+54)
#define LVM_GETEXTENDEDLISTVIEWSTYLE (LVM_FIRST+55)
#define LVM_SETCOLUMNORDERARRAY (LVM_FIRST+58)
#define LVM_GETCOLUMNORDERARRAY (LVM_FIRST+59)
#define LVM_GETHEADER (LVM_FIRST+31)
#define LVM_GETHOTCURSOR (LVM_FIRST+63)
#define LVM_GETHOTITEM (LVM_FIRST+61)
#define LVM_GETSUBITEMRECT (LVM_FIRST+56)
#define LVM_SETHOTCURSOR (LVM_FIRST+62)
#define LVM_SETHOTITEM (LVM_FIRST+60)
#define LVM_SETICONSPACING (LVM_FIRST+53)
#define LVM_SUBITEMHITTEST (LVM_FIRST+57)
#endif

#ifndef LVS_OWNERDATA
#define LVS_OWNERDATA 4096
#define LVS_EX_CHECKBOXES 4
#define LVS_EX_FULLROWSELECT 32
#define LVS_EX_GRIDLINES 1
#define LVS_EX_HEADERDRAGDROP 16
#define LVS_EX_ONECLICKACTIVATE 64
#define LVS_EX_SUBITEMIMAGES 2
#define LVS_EX_TRACKSELECT 8
#define LVS_EX_TWOCLICKACTIVATE 128
#define LVSICF_NOINVALIDATEALL	0x00000001
#define LVSICF_NOSCROLL	0x00000002
#endif

#ifndef TB_SETINDENT
#define TB_SETINDENT	(WM_USER+47)
#define TB_SETIMAGELIST	(WM_USER+48)
#define TB_GETIMAGELIST	(WM_USER+49)
#define TB_LOADIMAGES	(WM_USER+50)
#define TB_GETRECT	(WM_USER+51)
#define TB_SETHOTIMAGELIST	(WM_USER+52)
#define TB_GETHOTIMAGELIST	(WM_USER+53)
#define TB_SETDISABLEDIMAGELIST (WM_USER+54)
#define TB_GETDISABLEDIMAGELIST	(WM_USER+55)
#define TB_SETSTYLE	(WM_USER+56)
#define TB_GETSTYLE	(WM_USER+57)
#define TB_GETBUTTONSIZE	(WM_USER+58)
#define TB_SETBUTTONWIDTH	(WM_USER+59)
#define TB_SETMAXTEXTROWS	(WM_USER+60)
#define TB_GETTEXTROWS	(WM_USER+61)
#endif

#ifndef TBSTYLE_FLAT
#define TBSTYLE_FLAT 2048
#define TBSTYLE_LIST 4096
#define TBSTYLE_CUSTOMERASE 8192
#endif

AUDIO_CONVERTER::AUDIO_CONVERTER()
{
   TotalItem=0;
   CurrentItem=0;
   convert_exception=true;
   button_exception=true;
   timer_on=false;
   list_click=false;
   on_convert_ontop=false;
}


void AUDIO_CONVERTER::InitializeSettings()
{
  int temp;
  
  timer_on=false;
  
  temp=Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"maximize");
  
  if(temp==1)
    IsMaximize=true;
  else
    IsMaximize=false;
    
   temp=Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"column 0");
   if(temp==-1 || temp==0) temp=200;
      
       ListView_SetColumnWidth(ListViewHandle,0,temp);
      
   
   
   temp=Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"column 1");
   if(temp==-1 || temp==0) temp=60;  
     
      ListView_SetColumnWidth(ListViewHandle,1,temp);
     
   
   temp=Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"column 2");
   if(temp==-1 || temp==0) temp=80;  

      ListView_SetColumnWidth(ListViewHandle,2,temp);
     
   
   temp=Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"column 3");
   if(temp==-1 || temp==0) temp=370;  
     
      ListView_SetColumnWidth(ListViewHandle,3,temp);
     
     
   temp=Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"combo");
   if(temp==-1) temp=2;
     
      SendMessage(ComboBoxHandle,CB_SETCURSEL,temp,0);
      
    
}

void AUDIO_CONVERTER::StoreSettings()
{
   if(IsZoomed(AudioConverterHandle))
    Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"maximize",1);
   else
    Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"maximize",0);
    
    Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"column 0",ListView_GetColumnWidth(ListViewHandle,0));
    Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"column 1",ListView_GetColumnWidth(ListViewHandle,1));
    Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"column 2",ListView_GetColumnWidth(ListViewHandle,2));
    Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"column 3",ListView_GetColumnWidth(ListViewHandle,3));
    
    Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"combo",SendMessage(ComboBoxHandle,CB_GETCURSEL,0,0));
}

HWND AUDIO_CONVERTER::AudioConverterDialog()
{
    MSG msg;
    WNDCLASSEX wnd;
    HWND hWnd;
    
    wnd.cbSize=sizeof(WNDCLASSEX);
    wnd.style=CS_VREDRAW|CS_HREDRAW;
    wnd.lpfnWndProc=AudioConverterProc;
    wnd.cbClsExtra=0;
    wnd.cbWndExtra=0;
    wnd.hInstance=NULL;
    wnd.hIcon=LoadIcon(NULL,IDI_APPLICATION);
    wnd.hCursor=LoadCursor(NULL,IDC_ARROW);
    wnd.hbrBackground=(HBRUSH)(COLOR_3DFACE+1);
    wnd.lpszMenuName=NULL;
    wnd.lpszClassName="RBAudioConverter_Class";
    wnd.hIconSm=LoadIcon(NULL,IDI_APPLICATION);
    
    RegisterClassEx(&wnd);
    
    hWnd=CreateWindow("RBAudioConverter_Class","RB Audio Converter",
                       WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                       0,0,730,600,
                       NULL,NULL,NULL,NULL);
    
    AddMenu(hWnd);
    
    AudioConverterHandle=hWnd;
    
    return hWnd;
}




LRESULT CALLBACK AUDIO_CONVERTER::AudioConverterProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
        switch(msg)
        {
                   case WM_CREATE:
                         {
                          SendMessage(hwnd,WM_SETICON,ICON_SMALL,(LPARAM)LoadIconA(GetModuleHandle(NULL),MAKEINTRESOURCE(1213)));
                          SendMessage(hwnd,WM_SETICON,ICON_BIG,(LPARAM)LoadIconA(GetModuleHandle(NULL),MAKEINTRESOURCE(1213)));
                          
                          General.CenterWindow(GetDesktopWindow(),hwnd);
                          
                          DragAcceptFiles(hwnd,TRUE);
                           
                          AudioConverter.CreateToolbarControls(hwnd);
                          AudioConverter.CreateListViewControls(hwnd,5,75,700,200);
                          AudioConverter.CreateButtonControls(hwnd);
                          AudioConverter.CreateProgressBar(hwnd);
                          AudioConverter.CreateComboBox(hwnd);
                          AudioConverter.CreateStatusbarControl(hwnd);
                          
                          AudioConverter.InitializeSettings();
                          
                         }
                   break;
                   case WM_MEASUREITEM:
                        {
                        MEASUREITEMSTRUCT *mis=(MEASUREITEMSTRUCT*)lParam;
                        
                          if(!AudioConverter.list_click)
                            AC_ODMenu.ODMenu_OnMeasureItem(hwnd,mis);
                          else
                            ACN_ODMenu.ODMenu_OnMeasureItem(hwnd,mis);
                        }
                   break;
                   case WM_DRAWITEM:
                        {
                        DRAWITEMSTRUCT *dis=(DRAWITEMSTRUCT*)lParam;
                        
                        if(!AudioConverter.list_click)
                           AC_ODMenu.ODMenu_OnDrawItem(hwnd,dis);
                        else
                           ACN_ODMenu.ODMenu_OnDrawItem(hwnd,dis);
                        }
                   break;
                   case WM_COMMAND:
                        switch(LOWORD(wParam))
                        {
                         case IDC_AUDIO_ADDFILES:
                              {
                               AudioConverter.AddFiles();
                              }
                         break;
                        case IDC_AUDIO_ADDFOLDER:
                              {
                               AudioConverter.AddFolder();
                              }
                        break;
                        case IDC_AUDIO_EXPLORE:
                              {
                               AudioConverter.OnExplore();
                              }
                        break;
                        case IDC_AUDIO_REMOVE:
                              {
                              AudioConverter.OnRemove();        
                              }
                        break;
                        case IDC_AUDIO_REMOVEALL:
                              {
                              AudioConverter.OnRemoveAll();                 
                              }
                        break;
                        case IDC_AUDIO_CONVERT:
                              {
                              AudioConverter.OnConvert();
                              }
                        break;
                        case IDC_AUDIO_PAUSE:
                             {
                              AudioConverter.OnPause(); 
                             }
                        break;
                        case IDC_AUDIO_STOP:
                             {
                             AudioConverter.OnStop();
                             }
                        break;
                        case IDC_AUDIO_SETTINGS:
                              {
                              DialogBox(NULL,MAKEINTRESOURCE(IDD_SETTINGS),hwnd,(DLGPROC)Settings);
                              }
                        break;
                        case IDC_AUDIO_HELP:
                              {
                              char temp[MAX_PATH+20];
                              strcpy(temp,GetCommandLineA());
                              int i=0;
                              strtok(temp,"\"");
                              strrev(temp);
                              strtok(temp,"\"");
                              strrev(temp);
                              i=strlen(temp);
                              while(i>=0)
                               {
                                if(temp[i--]=='\\') break;
                               }
                               temp[i+1]='\0';
                              strcat(temp,"\\help.chm");
                              ShellExecuteA(NULL,"open",temp,NULL,NULL,SW_SHOWMAXIMIZED);           
                              }
                        break;
                        case IDC_AUDIO_EXIT:
                              {
                              AudioConverter.ExitConverter(hwnd);
                              }
                        break;
                        
                        case IDM_AUDIO_PROPERTIES:
                              {
                              AudioConverter.OnProperties();
                              }
                        break;
                        
                        case IDM_NOTIFY_AUDIO_EXPLORE:
                           {
                            AudioConverter.FolderExplore();
                           }
                         break;
                        
                        case IDC_AUDIO_ENCODER:
                             {
                              AudioConverter.GetEncoderDialog(hwnd);
                             }
                        break;
                        
                        
                        case IDM_AUDIO_ABOUT:
                             {
                              DialogBox(NULL,MAKEINTRESOURCE(IDD_RB_ABOUT),hwnd,(DLGPROC)About);
                             }
                        break;
                        
                        //MENU ITEMS
                        ////////////////////////////////////////////////////////
                        //ON TOP
                        case IDM_AUDIO_ONTOP_NEVER:
                             {
                              HMENU hViewMenu=GetSubMenu(AudioConverter.MenuHandle,1);
                              HMENU hOnTop=GetSubMenu(hViewMenu,0);
                              CheckMenuRadioItem(hOnTop,0,3,0,MF_BYPOSITION);
                              Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"ontop",0);
                              SetWindowPos(hwnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
                              AudioConverter.on_convert_ontop=false;
                             }
                        break;
                        
                        case IDM_AUDIO_ONTOP_ALWAYS:
                             {
                              HMENU hViewMenu=GetSubMenu(AudioConverter.MenuHandle,1);
                              HMENU hOnTop=GetSubMenu(hViewMenu,0);
                              CheckMenuRadioItem(hOnTop,0,3,2,MF_BYPOSITION);
                              Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"ontop",1);
                              SetWindowPos(hwnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
                              AudioConverter.on_convert_ontop=false;
                             }
                        break;
                        
                        case IDM_AUDIO_ONTOP_CONVERTING:
                             {
                              HMENU hViewMenu=GetSubMenu(AudioConverter.MenuHandle,1);
                              HMENU hOnTop=GetSubMenu(hViewMenu,0);
                              CheckMenuRadioItem(hOnTop,0,3,3,MF_BYPOSITION);
                              Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"ontop",2);
                              AudioConverter.on_convert_ontop=true;
                             }
                        break;
                        /////////////////////////////////////////////////////////////////
                        
                        
                        //TRANSPARENCY
                        ////////////////////////////////////////////////////////////////
                        
                        case IDM_AUDIO_TRANS_ZERO:
                             {
                              HMENU hViewMenu=GetSubMenu(AudioConverter.MenuHandle,1);
                              HMENU hTrans=GetSubMenu(hViewMenu,1);
                              CheckMenuRadioItem(hTrans,0,11,0,MF_BYPOSITION);
                              System.SetTransparency(hwnd,0);
                              SetWindowLong(hwnd,GWL_EXSTYLE,GetWindowLong(hwnd,GWL_EXSTYLE)&(~WS_EX_LAYERED));
                              Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"transparency",0);
                             }
                        break;
                        
                        case IDM_AUDIO_TRANS_TEN:
                             {
                              HMENU hViewMenu=GetSubMenu(AudioConverter.MenuHandle,1);
                              HMENU hTrans=GetSubMenu(hViewMenu,1);
                              CheckMenuRadioItem(hTrans,0,11,2,MF_BYPOSITION);
                              System.SetTransparency(hwnd,10);
                              Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"transparency",1);
                             }
                        break;
                        
                        case IDM_AUDIO_TRANS_TWENTY:
                             {
                              HMENU hViewMenu=GetSubMenu(AudioConverter.MenuHandle,1);
                              HMENU hTrans=GetSubMenu(hViewMenu,1);
                              CheckMenuRadioItem(hTrans,0,11,3,MF_BYPOSITION);
                              System.SetTransparency(hwnd,20);
                              Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"transparency",2);
                             }
                        break;
                        
                        case IDM_AUDIO_TRANS_THIRTY:
                             {
                              HMENU hViewMenu=GetSubMenu(AudioConverter.MenuHandle,1);
                              HMENU hTrans=GetSubMenu(hViewMenu,1);
                              CheckMenuRadioItem(hTrans,0,11,4,MF_BYPOSITION);
                              System.SetTransparency(hwnd,30);
                              Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"transparency",3);
                             }
                        break;
                        
                        case IDM_AUDIO_TRANS_FORTY:
                             {
                              HMENU hViewMenu=GetSubMenu(AudioConverter.MenuHandle,1);
                              HMENU hTrans=GetSubMenu(hViewMenu,1);
                              CheckMenuRadioItem(hTrans,0,11,5,MF_BYPOSITION);
                              System.SetTransparency(hwnd,40);
                              Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"transparency",4);
                             }
                        break;
                        
                        
                        case IDM_AUDIO_TRANS_FIFTY:
                             {
                              HMENU hViewMenu=GetSubMenu(AudioConverter.MenuHandle,1);
                              HMENU hTrans=GetSubMenu(hViewMenu,1);
                              CheckMenuRadioItem(hTrans,0,11,6,MF_BYPOSITION);
                              System.SetTransparency(hwnd,50);
                              Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"transparency",5);
                             }
                        break;
                        
                        
                        case IDM_AUDIO_TRANS_SIXTY:
                             {
                              HMENU hViewMenu=GetSubMenu(AudioConverter.MenuHandle,1);
                              HMENU hTrans=GetSubMenu(hViewMenu,1);
                              CheckMenuRadioItem(hTrans,0,11,7,MF_BYPOSITION);
                              System.SetTransparency(hwnd,60);
                              Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"transparency",6);
                             }
                        break;
                        
                        
                        case IDM_AUDIO_TRANS_SEVENTY:
                             {
                              HMENU hViewMenu=GetSubMenu(AudioConverter.MenuHandle,1);
                              HMENU hTrans=GetSubMenu(hViewMenu,1);
                              CheckMenuRadioItem(hTrans,0,11,8,MF_BYPOSITION);
                              System.SetTransparency(hwnd,70);
                              Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"transparency",7);
                             }
                        break;
                        
                        
                        case IDM_AUDIO_TRANS_EIGHTY:
                             {
                              HMENU hViewMenu=GetSubMenu(AudioConverter.MenuHandle,1);
                              HMENU hTrans=GetSubMenu(hViewMenu,1);
                              CheckMenuRadioItem(hTrans,0,11,9,MF_BYPOSITION);
                              System.SetTransparency(hwnd,80);
                              Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"transparency",8);
                             }
                        break;
                        
                        
                        case IDM_AUDIO_TRANS_NINTY:
                             {
                              HMENU hViewMenu=GetSubMenu(AudioConverter.MenuHandle,1);
                              HMENU hTrans=GetSubMenu(hViewMenu,1);
                              CheckMenuRadioItem(hTrans,0,11,10,MF_BYPOSITION);
                              System.SetTransparency(hwnd,90);
                              Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"transparency",9);
                             }
                        break;
                        
                        ///////////////////////////////////////////////////////////////
                        
                        //MENU COLOR
                        ///////////////////////////////////////////////////////////////
                        
                        
                        case IDM_AUDIO_MAIN_COLOR_DEFAULT:
                             {
                              HMENU hViewMenu=GetSubMenu(AudioConverter.MenuHandle,1);
                              HMENU hColor=GetSubMenu(hViewMenu,3);
                              HMENU hMain=GetSubMenu(hColor,0);
                              CheckMenuRadioItem(hMain,0,4,0,MF_BYPOSITION);
                              AC_ODMenu.SetDefaultMenu();
                              Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"mcolor",0);
                             }
                        break;
                        
                        
                        case IDM_AUDIO_MAIN_COLOR_RED:
                             {
                              HMENU hViewMenu=GetSubMenu(AudioConverter.MenuHandle,1);
                              HMENU hColor=GetSubMenu(hViewMenu,3);
                              HMENU hMain=GetSubMenu(hColor,0);
                              CheckMenuRadioItem(hMain,0,4,2,MF_BYPOSITION);
                              AC_ODMenu.SetRedMenu();
                              Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"mcolor",1);
                             }
                        break;
                        
                        
                        case IDM_AUDIO_MAIN_COLOR_GREEN:
                             {
                              HMENU hViewMenu=GetSubMenu(AudioConverter.MenuHandle,1);
                              HMENU hColor=GetSubMenu(hViewMenu,3);
                              HMENU hMain=GetSubMenu(hColor,0);
                              CheckMenuRadioItem(hMain,0,4,3,MF_BYPOSITION);
                              AC_ODMenu.SetGreenMenu();
                              Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"mcolor",2);
                             }
                        break;
                        
                        
                        case IDM_AUDIO_MAIN_COLOR_BLUE:
                             {
                              HMENU hViewMenu=GetSubMenu(AudioConverter.MenuHandle,1);
                              HMENU hColor=GetSubMenu(hViewMenu,3);
                              HMENU hMain=GetSubMenu(hColor,0);
                              CheckMenuRadioItem(hMain,0,4,4,MF_BYPOSITION);
                              AC_ODMenu.SetBlueMenu();
                              Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"mcolor",3);
                             }
                        break;
                        
                        
                        ////////////////////////////////////////////////////////////////
                        
                        case IDM_AUDIO_POPUP_COLOR_DEFAULT:
                             {
                              HMENU hViewMenu=GetSubMenu(AudioConverter.MenuHandle,1);
                              HMENU hColor=GetSubMenu(hViewMenu,3);
                              HMENU hPopup=GetSubMenu(hColor,1);
                              CheckMenuRadioItem(hPopup,0,4,0,MF_BYPOSITION);
                              ACN_ODMenu.SetDefaultMenu();
                              Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"pcolor",0);
                             }
                        break;
                        
                        
                        case IDM_AUDIO_POPUP_COLOR_RED:
                             {
                              HMENU hViewMenu=GetSubMenu(AudioConverter.MenuHandle,1);
                              HMENU hColor=GetSubMenu(hViewMenu,3);
                              HMENU hPopup=GetSubMenu(hColor,1);
                              CheckMenuRadioItem(hPopup,0,4,2,MF_BYPOSITION);
                              ACN_ODMenu.SetRedMenu();
                              Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"pcolor",1);
                             }
                        break;
                        
                        
                        case IDM_AUDIO_POPUP_COLOR_GREEN:
                             {
                              HMENU hViewMenu=GetSubMenu(AudioConverter.MenuHandle,1);
                              HMENU hColor=GetSubMenu(hViewMenu,3);
                              HMENU hPopup=GetSubMenu(hColor,1);
                              CheckMenuRadioItem(hPopup,0,4,3,MF_BYPOSITION);
                              ACN_ODMenu.SetGreenMenu();
                              Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"pcolor",2);
                             }
                        break;
                        
                        
                        case IDM_AUDIO_POPUP_COLOR_BLUE:
                             {
                              HMENU hViewMenu=GetSubMenu(AudioConverter.MenuHandle,1);
                              HMENU hColor=GetSubMenu(hViewMenu,3);
                              HMENU hPopup=GetSubMenu(hColor,1);
                              CheckMenuRadioItem(hPopup,0,4,4,MF_BYPOSITION);
                              ACN_ODMenu.SetBlueMenu();
                              Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"pcolor",3);
                             }
                        break;
                        
                        //////////////////////////////////////////////////////////////
                        
                        //////////////////////////////////////////////////////////////
                        //AFTER CONVERSION...........................................
                        
                        case IDM_AUDIO_CONVERTER_NOTHING:
                             {
                              HMENU hConvertMenu=GetSubMenu(AudioConverter.MenuHandle,2);
                              HMENU hConversion=GetSubMenu(hConvertMenu,5);
                              CheckMenuRadioItem(hConversion,0,6,0,MF_BYPOSITION);
                              Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"aconversion",0);
                             }
                        break;
                        
                        
                         case IDM_AUDIO_CONVERTER_CLOSE_C:
                             {
                              HMENU hConvertMenu=GetSubMenu(AudioConverter.MenuHandle,2);
                              HMENU hConversion=GetSubMenu(hConvertMenu,5);
                              CheckMenuRadioItem(hConversion,0,6,2,MF_BYPOSITION);
                              Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"aconversion",1);
                             }
                        break;
                        
                        
                       /*  case IDM_AUDIO_CONVERTER_CLOSE_P:
                             {
                              HMENU hConvertMenu=GetSubMenu(AudioConverter.MenuHandle,2);
                              HMENU hConversion=GetSubMenu(hConvertMenu,5);
                              CheckMenuRadioItem(hConversion,0,7,3,MF_BYPOSITION);
                              Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"aconversion",2);
                             }
                        break;*/
                        
                        
                         case IDM_AUDIO_CONVERTER_LOGOFF:
                             {
                              HMENU hConvertMenu=GetSubMenu(AudioConverter.MenuHandle,2);
                              HMENU hConversion=GetSubMenu(hConvertMenu,5);
                              CheckMenuRadioItem(hConversion,0,6,3,MF_BYPOSITION);
                              Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"aconversion",3);
                             }
                        break;
                        
                        
                         case IDM_AUDIO_CONVERTER_STANDBY:
                             {
                              HMENU hConvertMenu=GetSubMenu(AudioConverter.MenuHandle,2);
                              HMENU hConversion=GetSubMenu(hConvertMenu,5);
                              CheckMenuRadioItem(hConversion,0,6,4,MF_BYPOSITION);
                              Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"aconversion",4);
                             }
                        break;
                        
                        
                         case IDM_AUDIO_CONVERTER_RESTART:
                             {
                              HMENU hConvertMenu=GetSubMenu(AudioConverter.MenuHandle,2);
                              HMENU hConversion=GetSubMenu(hConvertMenu,5);
                              CheckMenuRadioItem(hConversion,0,6,5,MF_BYPOSITION);
                              Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"aconversion",5);
                             }
                        break;
                        
                        
                         case IDM_AUDIO_CONVERTER_SHUTDOWN:
                             {
                              HMENU hConvertMenu=GetSubMenu(AudioConverter.MenuHandle,2);
                              HMENU hConversion=GetSubMenu(hConvertMenu,5);
                              CheckMenuRadioItem(hConversion,0,6,6,MF_BYPOSITION);
                              Registry.WriteNumberSetting(AUDIO_CONVERTER_PATH,"aconversion",6);
                             }
                        break;
                        
                        /////////////////////////////////////////////////////////////
                        
                        }
                   break;
                   case WM_TIMER:
                        {
                        AudioConverter.OnTimer(hwnd);
                        }
                   break;
                   case WM_DROPFILES:
                        {
                        AudioConverter.OnDropFile(hwnd,wParam);
                        }
                   break;
                   case WM_SIZE:
                        {
                        AudioConverter.OnSize(hwnd,wParam,lParam);
                        }
                   break;
                   case WM_NOTIFY:
                        {
                        AudioConverter.OnNotify(hwnd,lParam);
                        }
                   break;
                   case WM_GETMINMAXINFO:
                        {
                        ((LPMINMAXINFO)lParam)->ptMinTrackSize.x=730;
                        ((LPMINMAXINFO)lParam)->ptMinTrackSize.y=600;
                        }
                   break;
                   case WM_SHOWWINDOW:
                        {
                        if(AudioConverter.IsMaximize)
                           ShowWindow(hwnd,SW_SHOWMAXIMIZED);
                        }
                   break;
                   case WM_CTLCOLORSTATIC:
                        SetTextColor((HDC)wParam,RGB(255,0,0));
                        SetBkMode((HDC)wParam,TRANSPARENT);
                        return (LONG)(COLOR_3DFACE+1);
                   break;
                   case WM_CLOSE:
                        AudioConverter.ExitConverter(hwnd);
                   break;
                   case WM_DESTROY:
                         PostQuitMessage(0);
                   break;
                   default:
                        return DefWindowProc(hwnd,msg,wParam,lParam);
        }
return FALSE;
}


void AUDIO_CONVERTER::OnNotify(HWND hwnd,LPARAM lParam)
{
     NMHDR *hdr=(NMHDR*)lParam;
     
     HWND hwndFrom=hdr->hwndFrom;
     UINT code=hdr->code;
     
     if(hwndFrom==ListViewHandle)
      {
        switch(code)
         {
           case NM_RCLICK:
            {
               int index=ListView_GetNextItem(ListViewHandle,(WPARAM)-1,LVNI_SELECTED);
               
               if(index!=-1)
                {
                  list_click=true;
                  POINT pt;
                  GetCursorPos(&pt);
                  HMENU hMenu=CreatePopupMenu();
                  
                  //AppendMenu(hMenu,MFT_STRING,IDM_NOTIFY_AUDIO_PLAY,"&Play\tEnter");
                  //AppendMenu(hMenu,MFT_STRING,IDM_NOTIFY_AUDIO_QUEUE,"&Add To Queue");
                  //AppendMenu(hMenu,MF_SEPARATOR,IDC_STATIC,"");
                  AppendMenu(hMenu,MFT_STRING,IDM_NOTIFY_AUDIO_EXPLORE,"Explore");
                  AppendMenu(hMenu,MFT_STRING,IDC_AUDIO_REMOVE,"&Remove\tCtrl+R");
                  AppendMenu(hMenu,MFT_STRING,IDC_AUDIO_REMOVEALL,"R&emove &All\tCtrl+A");
                  AppendMenu(hMenu,MF_SEPARATOR,IDC_STATIC,"");
                  AppendMenu(hMenu,MFT_STRING,IDM_AUDIO_PROPERTIES,"Prop&erties\tCtrl+E");
                  
                  //CODE TO PLACE FOR MENU BACKGROUND CHANGE
                  
                  if(Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"pcolor")==0 ||
                     Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"pcolor")==-1)
                     ACN_ODMenu.SetDefaultMenu();
                  
                  if(Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"pcolor")==1)
                     ACN_ODMenu.SetRedMenu();
                     
                  if(Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"pcolor")==2)
                     ACN_ODMenu.SetGreenMenu();
                     
                  if(Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"pcolor")==3)
                     ACN_ODMenu.SetBlueMenu();
                  
                  ////////////////////////////////////////////////////////
                  
                  
                  ACN_ODMenu.ConvertMenu(hMenu,true);
                  
                  //ACN_ODMenu.AddMenuIcon(NULL,137);
                  //ACN_ODMenu.AddMenuIcon(NULL,225);
                  //ACN_ODMenu.AddMenuIcon(NULL,50);
                  ACN_ODMenu.AddMenuIcon(NULL,126);
                  ACN_ODMenu.AddMenuIcon(LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_TOOLBAR_REMOVE)),0);
                  ACN_ODMenu.AddMenuIcon(LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_TOOLBAR_REMOVE)),0);
                  ACN_ODMenu.AddMenuIcon(NULL,50);
                  ACN_ODMenu.AddMenuIcon(NULL,19);
                  
                  
                  //ENABLING AND DISABLING MENUITEMS
                  
                  if(!IsWindowEnabled(ConvertButtonHandle))
                    {
                     EnableMenuItem(hMenu,IDC_AUDIO_REMOVE,MF_BYCOMMAND|MF_GRAYED);
                     EnableMenuItem(hMenu,IDC_AUDIO_REMOVEALL,MF_BYCOMMAND|MF_GRAYED);
                    } 
                  TrackPopupMenu(hMenu,TPM_LEFTALIGN,pt.x,pt.y,0,hwnd,NULL);
                  list_click=false;
                  ACN_ODMenu.RecreateImage();
                  DestroyMenu(hMenu);
                }
            }
           break;
           
           case NM_RETURN:
           case NM_DBLCLK:
            {
               int index=ListView_GetNextItem(ListViewHandle,(WPARAM)-1,LVNI_SELECTED);
               
               if(index!=-1)
                {
                  MessageBox(0,"","",0);
                }
             }
         }
      }
}


/******************************************************************

Statusbar Controls

******************************************************************/

void AUDIO_CONVERTER::CreateStatusbarControl(HWND hwnd)
{
     int iStatusWidths[] = {500,-1} ;
      
     StatusbarHandle=CreateWindowEx(0,"msctls_statusbar32", NULL,
            WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, 0, 0, 0, 0,
            hwnd, (HMENU)NULL, NULL , NULL);
     SendMessage(StatusbarHandle, SB_SETPARTS, 2, (LPARAM)iStatusWidths);
     SendMessage(StatusbarHandle, SB_SETTEXT, 0, (LPARAM)"RB Audio Converter");
     SendMessage(StatusbarHandle, SB_SETTEXT, 1, (LPARAM)"0 Files");
}





/******************************************************************

ListView Controls

******************************************************************/

void AUDIO_CONVERTER::CreateListViewControls(HWND hwnd,int x,int y,int cx, int cy)
{
      ListViewHandle=CreateWindowExA(0,"SysListView32",NULL,
                            WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_SINGLESEL|LVS_SHOWSELALWAYS,
                             x,y,cx,cy,hwnd,NULL,NULL,NULL);
                             
      SendMessage(ListViewHandle,LVM_SETEXTENDEDLISTVIEWSTYLE,0,
                            LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
      
      ListViewImageList=ImageList_Create(20,20,ILC_COLOR32,1,0);
      ImageList_AddIcon(ListViewImageList,LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_LISTVIEW_MUSIC)));
      ImageList_AddIcon(ListViewImageList,LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_LISTVIEW_DONE)));
      ImageList_AddIcon(ListViewImageList,LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_LISTVIEW_NOTDONE)));
      
      ListView_SetImageList(ListViewHandle,ListViewImageList,LVSIL_SMALL);
      
      AddListViewColumn(ListViewHandle ,0,200,"FileName");
      AddListViewColumn(ListViewHandle ,1,60,"Format");
      AddListViewColumn(ListViewHandle ,2,80,"Size");
      AddListViewColumn(ListViewHandle ,3,370,"FilePath");
}

void AUDIO_CONVERTER::AddListViewColumn(HWND hwnd,int column,int cx,LPSTR pszText,UINT mask,int fmt)
{
     LV_COLUMN col;
     col.mask=mask;
     col.fmt=fmt;
     col.cx=cx;
     col.pszText=pszText;
     ListView_InsertColumn(hwnd,column,&col);
} 

void AUDIO_CONVERTER::AddListViewItem(HWND hwnd,int iImage,LPSTR szFileName,...)
{
     int cnt;
     int i=0,j=0;
     char buffer[256];
     char temp[64];
     int iSubItem=0;
     int exception=0;
     LV_ITEM item;
     
     cnt=SendMessage(hwnd,LVM_GETITEMCOUNT,0,0);
          
     va_list args;
     va_start(args,szFileName);
     _vsnprintf(buffer,sizeof buffer,szFileName,args);
     va_end(args);
     
     while(buffer[i]!='\0')
      {
       temp[j++]=buffer[i];
       
       if(buffer[i+1]=='|')
        {
         temp[j]='\0';
         item.iItem=cnt;
         item.pszText=temp;
         
         if(iSubItem==0)
           {
            item.mask=LVIF_TEXT|LVIF_IMAGE;
            item.iSubItem=iSubItem++;
            item.iImage=iImage;
            ListView_InsertItem(hwnd,&item);
           }
         else
           {
            item.iSubItem=iSubItem++;
            ListView_SetItem(hwnd,&item);
            exception=i+2;
           }
         temp[0]='\0';
         j=0;
         i++;
        }
        i++;
      }
      
      temp[0]='\0';
      j=0;
      
      while(buffer[exception]!='\0')
       {
            temp[j++]=buffer[exception++];                        
       }
       temp[j]='\0';
       
       item.iItem=cnt;
       item.pszText=temp;
       item.iSubItem=iSubItem;
       ListView_SetItem(hwnd,&item);
}

char* AUDIO_CONVERTER::GetListViewColumnText(HWND hwnd,int iItem,int iSubItem)
{
     static char buffer[MAX_PATH];
     
     ListView_GetItemText(hwnd,iItem,iSubItem,buffer,sizeof buffer + 1);
     
     return buffer;
}

char* AUDIO_CONVERTER::GetPathFromListView(HWND hwnd,int iItem)
{
     static char path[MAX_PATH];
     
     strcpy(path,GetListViewColumnText(hwnd,iItem,3));
     strcat(path,GetListViewColumnText(hwnd,iItem,0));
     strcat(path,GetListViewColumnText(hwnd,iItem,1));
     
     return path;
}

void AUDIO_CONVERTER::ListViewChangeImage(HWND hwnd,int nCol,int iImage)
{
         LV_ITEM item;
         item.iItem=nCol;
         item.mask=LVIF_IMAGE;
         item.iSubItem=0;
         item.iImage=iImage;
         ListView_SetItem(hwnd,&item);
}

void AUDIO_CONVERTER::ListView_SetItemSelect(HWND hwnd,int nIndex,bool bSelected,bool bFocused)
{
     LV_ITEM  item;
     DWORD    dSelected,dFocused;
     int      nSize;
     
     if(bSelected)
        dSelected=LVIS_SELECTED;
     else
        dSelected=0;
        
     if(bFocused)
        dFocused=LVIS_FOCUSED;
     else
        dFocused=0;
     
     item.mask=LVIF_STATE;
     item.iItem=nIndex;
     item.state=dSelected|dFocused;
     item.stateMask=LVIS_SELECTED|dFocused;
     
     SetFocus(hwnd);
     SendMessage(hwnd,LVM_SETITEMSTATE,nIndex,(LPARAM)&item);
     
}

void AUDIO_CONVERTER::FindFile_Recursive(char *szPath,char *extension=".")
{
	WIN32_FIND_DATA	FindFileData,FindMusicFile;
	HANDLE			hFind,hMusic;
	char			szTemp[MAX_PATH+1];
	char 		    szPathPrev[MAX_PATH+1];
	char            szExt[20];
	
	strcpy(szTemp,szPath);
	strcpy(szPathPrev,szTemp);
	strcat(szPathPrev,"\\");
	strcat(szTemp,"\\*.*");

	hFind = FindFirstFile(szTemp,&FindFileData);

	while (hFind != INVALID_HANDLE_VALUE)
	{
		if (!(!strcmp(FindFileData.cFileName, ".") || !strcmp(FindFileData.cFileName, "..")))
		{  
			strcpy(szTemp,szPathPrev);
			strcat(szTemp,FindFileData.cFileName);
			
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			  {                               
                FindFile_Recursive(szTemp,extension);
              }
            else
              {
               if(!strcmp(".",extension))
                  SetPathToList(szTemp);
               else
                 {
                  _splitpath(szTemp,NULL,NULL,NULL,szExt);
                  if(!stricmp(szExt,extension))
                    SetPathToList(szTemp);
                   
                 }
              }
             
		 }
		if (!FindNextFile(hFind,&FindFileData))
			break;
	}
	FindClose(hFind);
}


void AUDIO_CONVERTER::SetPathToList(char *szBuffer)
{
     WIN32_FIND_DATA   wfd;
     HANDLE            hFile;
     char              szFileName[MAX_PATH+1];
     char              szExt[20];
     char              szDrive[5];
     char              szDir[MAX_PATH+1];
     DWORD             nSize;
     
     hFile=FindFirstFile(szBuffer,&wfd);
      
     nSize=(((__int64)(wfd.nFileSizeHigh) << 32) | (__int64)(wfd.nFileSizeLow));
        
      
     if(!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
      {
       _splitpath(szBuffer,szDrive,szDir,szFileName,szExt);
       
       if(!strcmp(szDrive,"")) strcpy(szDrive,"unknown");
       if(!strcmp(szDir,"")) strcpy(szDir,"unknown");
       if(!strcmp(szFileName,"")) strcpy(szFileName,"unknown");
       if(!strcmp(szExt,"")) strcpy(szExt,"unknown");
       
        if(strcmp(szExt,"unknown"))
       AddListViewItem(ListViewHandle,0,"%s|%s|%d|%s%s",szFileName,szExt,nSize,szDrive,szDir);
     }
     FindClose(hFile);
     
     wsprintf(szStatus,"%d Files",ListView_GetItemCount(ListViewHandle));
     SendMessage(StatusbarHandle, SB_SETTEXT, 1, (LPARAM)szStatus);
}



/******************************************************************

Load Plugin

******************************************************************/

void AUDIO_CONVERTER::LoadPlugin()
{
     WIN32_FIND_DATA  wfd;
     char             temp[MAX_PATH+1];
     char             path[MAX_PATH+30];
     
     
     strcpy(path,General.GetExecutableDir());
     strcat(path,"\\plugins\\*.dll");
     
     HANDLE hFile=FindFirstFile(path,&wfd);
     
     while(hFile!=INVALID_HANDLE_VALUE)
      {
       strcpy(temp,"plugins\\");
       strcat(temp,wfd.cFileName);
       BASS_PluginLoad(temp,0);
       if(!FindNextFile(hFile,&wfd)) break;
      }
      
      CloseHandle(hFile);
}

/******************************************************************

Toolbar Controls

******************************************************************/

void AUDIO_CONVERTER::CreateToolbarControls(HWND hwnd)
{
    
    StaticFont1=CreateFontA(15,8,0,0,0,0,0,0,0,0,0,0,0,"");
    StaticFont2=CreateFontA(14,6,0,0,0,0,0,0,0,0,0,0,0,"");
    StaticFont3=CreateFontA(15,6,0,0,0,0,0,0,0,0,0,0,0,"");
    
    ToolbarHandle=CreateWindowEx(WS_EX_CLIENTEDGE,"toolbarwindow32",NULL,
                            WS_CHILD|WS_VISIBLE|TBSTYLE_TOOLTIPS|CCS_TOP|WS_BORDER,
                                                 0,0,0,0,hwnd,NULL,NULL,NULL);
    
    TBBUTTON tb[12];
    SendMessage(ToolbarHandle,TB_BUTTONSTRUCTSIZE,sizeof(TBBUTTON),0);
    
    ToolbarImageList=ImageList_Create(50,45,ILC_COLOR32,1,0);
    ImageList_AddIcon(ToolbarImageList,LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_TOOLBAR_ADDFILES)));
    ImageList_AddIcon(ToolbarImageList,LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_TOOLBAR_ADDFOLDER)));
    ImageList_AddIcon(ToolbarImageList,LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_TOOLBAR_OUTPUTFOLDER)));
    ImageList_AddIcon(ToolbarImageList,LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_TOOLBAR_REMOVE)));
    ImageList_AddIcon(ToolbarImageList,LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_TOOLBAR_CONVERT)));
    ImageList_AddIcon(ToolbarImageList,LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_TOOLBAR_SETTINGS)));
    ImageList_AddIcon(ToolbarImageList,LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_TOOLBAR_HELP)));
    ImageList_AddIcon(ToolbarImageList,LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_TOOLBAR_EXIT)));
                            
                            
    SendMessage(ToolbarHandle,TB_SETIMAGELIST,0,(LPARAM)ToolbarImageList);
    
    ZeroMemory(tb,sizeof tb);
   
    AddToolbarButton(ToolbarHandle,tb[0],0,0,"Add Files",IDC_AUDIO_ADDFILES);
    AddToolbarButton(ToolbarHandle,tb[1],1,1,"Add Folder",IDC_AUDIO_ADDFOLDER);
    AddToolbarButton(ToolbarHandle,tb[2],2,2,"Output Folder",IDC_AUDIO_EXPLORE);
                           
    AddToolbarButton(ToolbarHandle,tb[3]);
                           
    AddToolbarButton(ToolbarHandle,tb[4],3,3,"Remove",IDC_AUDIO_REMOVE);
    AddToolbarButton(ToolbarHandle,tb[5],3,4,"Remove All",IDC_AUDIO_REMOVEALL);
                           
    AddToolbarButton(ToolbarHandle,tb[6]);
                           
    AddToolbarButton(ToolbarHandle,tb[7],4,5,"Convert",IDC_AUDIO_CONVERT);
    AddToolbarButton(ToolbarHandle,tb[8],5,6,"Settings",IDC_AUDIO_SETTINGS);
                           
    AddToolbarButton(ToolbarHandle,tb[9]);
                           
    AddToolbarButton(ToolbarHandle,tb[10],6,7,"Help",IDC_AUDIO_HELP);
    AddToolbarButton(ToolbarHandle,tb[11],7,8,"Exit",IDC_AUDIO_EXIT);
            
    SendMessage(ToolbarHandle,TB_AUTOSIZE,0,0);
    SendMessage(ToolbarHandle,TB_ADDBUTTONS,12,(LPARAM)&tb);
    SendMessage(ToolbarHandle,TB_SETSTYLE,0,SendMessage(ToolbarHandle,TB_GETSTYLE,0,0)|TBSTYLE_FLAT); 
    SendMessage(ToolbarHandle,TB_SETINDENT,10,0);
}

void AUDIO_CONVERTER::AddToolbarButton(HWND hwnd,TBBUTTON &tb,int iBitmap,int iString,LPSTR szText,int idCommand)
{                           
     if(szText==NULL)
      {
       tb.iBitmap=6;
       tb.fsStyle=TBSTYLE_SEP;
      } 
     else
       {
       tb.iBitmap=iBitmap;
       tb.fsState=TBSTATE_ENABLED;
       tb.fsStyle=TBSTYLE_BUTTON;
       tb.idCommand=idCommand;
       tb.iString=iString;
       SendMessage(hwnd,TB_ADDSTRING,iString,(LPARAM)szText);   
       }
}


/******************************************************************

Button Controls

******************************************************************/



void AUDIO_CONVERTER::CreateButtonControls(HWND hwnd)
{
    ConvertImageList=ImageList_Create(32,32,ILC_COLOR32,2,0);
    ImageList_AddIcon(ConvertImageList,LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_TOOLBAR_CONVERT)));
    
    PauseImageList=ImageList_Create(32,32,ILC_COLOR32,2,0);
    ImageList_AddIcon(PauseImageList,ExtractIcon(NULL,"%systemroot%\\system32\\shell32.dll",109));
    
    StopImageList=ImageList_Create(32,32,ILC_COLOR32,2,0);
    ImageList_AddIcon(StopImageList,LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_TOOLBAR_EXIT)));
    
    
    ConvertButtonHandle=Button.CreateImageButton("Convert ",10,435,160,50,hwnd,IDC_AUDIO_CONVERT,ConvertImageList,20,1,1,1,0);
    PauseButtonHandle=Button.CreateImageButton("Pause ",300,435,160,50,hwnd,IDC_AUDIO_PAUSE,PauseImageList,10,1,1,1,0);
    StopButtonHandle=Button.CreateImageButton("Stop ",560,435,160,50,hwnd,IDC_AUDIO_STOP,StopImageList,20,1,1,1,0);
    
    SendMessage(ConvertButtonHandle,WM_SETFONT,(WPARAM)StaticFont1,0);
    SendMessage(PauseButtonHandle,WM_SETFONT,(WPARAM)StaticFont1,0);
    SendMessage(StopButtonHandle,WM_SETFONT,(WPARAM)StaticFont1,0);
    EnableWindow(PauseButtonHandle,false);
    EnableWindow(StopButtonHandle,false);
}



/******************************************************************

Progress Section

******************************************************************/

void AUDIO_CONVERTER::CreateProgressBar(HWND hwnd)
{
     
  ProgressGroupHandle=CreateWindowEx(0,"button","Progress",WS_CHILD|WS_VISIBLE|BS_GROUPBOX,5,290,560,130,hwnd,NULL,NULL,NULL); 
  
  StaticHandle1=CreateWindowEx(0,"static","Current",WS_CHILD|WS_VISIBLE,15,327,65,20,hwnd,NULL,NULL,NULL);
  ProgressHandle=CreateWindowExA(0,"msctls_progress32",NULL,WS_CHILD|WS_VISIBLE,85,325,350,20,hwnd,NULL,NULL,NULL); 
  TimeHandle1=CreateWindowEx(0,"static","00:00:00 / 00:00:00",WS_CHILD|WS_VISIBLE,440,327,115,30,hwnd,NULL,NULL,NULL);
  
  StaticHandle2=CreateWindowEx(0,"static","Over All",WS_CHILD|WS_VISIBLE,15,377,60,20,hwnd,NULL,NULL,NULL);
  OverallProgressHandle=CreateWindowExA(0,"msctls_progress32",NULL,WS_CHILD|WS_VISIBLE,85,375,350,20,hwnd,NULL,NULL,NULL); 
  TimeHandle2=CreateWindowEx(0,"static","00:00:00 / 00:00:00",WS_CHILD|WS_VISIBLE,440,377,115,30,hwnd,NULL,NULL,NULL);
  
  SendMessage(StaticHandle1,WM_SETFONT,(WPARAM)StaticFont1,0);
  SendMessage(StaticHandle2,WM_SETFONT,(WPARAM)StaticFont1,0);
  SendMessage(TimeHandle1,WM_SETFONT,(WPARAM)StaticFont2,0);
  SendMessage(TimeHandle2,WM_SETFONT,(WPARAM)StaticFont2,0);
}


/******************************************************************

FORMAT SECTION

******************************************************************/

void AUDIO_CONVERTER::CreateComboBox(HWND hwnd)
{    
     FormatGroupHandle=CreateWindowEx(0,"button","Format",WS_CHILD|WS_VISIBLE|BS_GROUPBOX,560,290,150,130,hwnd,NULL,NULL,NULL); 
     ComboBoxHandle=CreateWindowEx(0,"combobox",NULL,WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST,580,340,80,20,hwnd,NULL,NULL,NULL); 
     StaticHandle3=CreateWindowEx(0,"static","Output Format:",WS_CHILD|WS_VISIBLE,580,320,85,20,hwnd,NULL,NULL,NULL);
     EncoderButtonHandle=CreateWindowEx(0,"button","Encoder Settings...",WS_CHILD|WS_VISIBLE,575,380,140,30,hwnd,(HMENU)IDC_AUDIO_ENCODER,NULL,NULL); 
     
     SendMessage(StaticHandle3,WM_SETFONT,(WPARAM)StaticFont2,0);
     SendMessage(EncoderButtonHandle,WM_SETFONT,(WPARAM)StaticFont1,0);
     SendMessage(ComboBoxHandle,WM_SETFONT,(WPARAM)StaticFont1,0);
     
     SendMessage(ComboBoxHandle,CB_ADDSTRING,0,(LPARAM)".aac");
     SendMessage(ComboBoxHandle,CB_ADDSTRING,1,(LPARAM)".flac");
     SendMessage(ComboBoxHandle,CB_ADDSTRING,2,(LPARAM)".mp3");
     SendMessage(ComboBoxHandle,CB_ADDSTRING,3,(LPARAM)".mpc");
     //SendMessage(ComboBoxHandle,CB_ADDSTRING,4,(LPARAM)".ogg");
     //SendMessage(ComboBoxHandle,CB_ADDSTRING,5,(LPARAM)".spx");
}


/*****************************************************************************

OnPause

*****************************************************************************/

void AUDIO_CONVERTER::OnPause()
{
     if(timer_on)
       {
       KillTimer(AudioConverterHandle,0);
       SendMessage(PauseButtonHandle,WM_SETTEXT,0,(LPARAM)"Resume");
       SendMessage(StatusbarHandle, SB_SETTEXT, 0, (LPARAM)"Audio conversion paused");
       }
     else
       {
       SetTimer(AudioConverterHandle,0,0,0);
       SendMessage(PauseButtonHandle,WM_SETTEXT,0,(LPARAM)"Pause");
       SendMessage(StatusbarHandle, SB_SETTEXT, 0, (LPARAM)"Audio conversion resumed");
       }
       
     if(timer_on)
       timer_on=false;
     else
       timer_on=true;
}


/*****************************************************************************

OnStop

*****************************************************************************/

void AUDIO_CONVERTER::OnStop()
{
     SendMessage(ProgressHandle,PBM_SETPOS,0,0);
     SendMessage(OverallProgressHandle,PBM_SETPOS,0,0);
     SendMessage(TimeHandle1,WM_SETTEXT,0,(LPARAM)"00:00:00 / 00:00:00");
     SendMessage(TimeHandle2,WM_SETTEXT,0,(LPARAM)"00:00:00 / 00:00:00");
     
     timer_on=false;
     KillTimer(AudioConverterHandle,0);
     
     BASS_Encode_Stop(nEncode);
     BASS_StreamFree(nChannel);
     
     HMENU hMenu=GetMenu(AudioConverterHandle);
     EnableMenuItem(GetSubMenu(hMenu,0),IDC_AUDIO_ADDFILES,MF_BYCOMMAND|MF_ENABLED);
     EnableMenuItem(GetSubMenu(hMenu,0),IDC_AUDIO_ADDFOLDER,MF_BYCOMMAND|MF_ENABLED);
     EnableMenuItem(GetSubMenu(hMenu,0),IDC_AUDIO_REMOVE,MF_BYCOMMAND|MF_ENABLED);
     EnableMenuItem(GetSubMenu(hMenu,0),IDC_AUDIO_REMOVEALL,MF_BYCOMMAND|MF_ENABLED);
     EnableMenuItem(GetSubMenu(hMenu,2),IDC_AUDIO_CONVERT,MF_BYCOMMAND|MF_ENABLED);
     EnableMenuItem(GetSubMenu(hMenu,2),IDC_AUDIO_SETTINGS,MF_BYCOMMAND|MF_ENABLED);
     EnableMenuItem(GetSubMenu(hMenu,2),IDC_AUDIO_ENCODER,MF_BYCOMMAND|MF_ENABLED);
     
     SendMessage(ToolbarHandle,TB_ENABLEBUTTON,(WPARAM)IDC_AUDIO_CONVERT,(LPARAM)MAKELONG(true,0));
     SendMessage(ToolbarHandle,TB_ENABLEBUTTON,(WPARAM)IDC_AUDIO_ADDFILES,(LPARAM)MAKELONG(true,0));
     SendMessage(ToolbarHandle,TB_ENABLEBUTTON,(WPARAM)IDC_AUDIO_ADDFOLDER,(LPARAM)MAKELONG(true,0));
     SendMessage(ToolbarHandle,TB_ENABLEBUTTON,(WPARAM)IDC_AUDIO_REMOVE,(LPARAM)MAKELONG(true,0));
     SendMessage(ToolbarHandle,TB_ENABLEBUTTON,(WPARAM)IDC_AUDIO_REMOVEALL,(LPARAM)MAKELONG(true,0));
     SendMessage(ToolbarHandle,TB_ENABLEBUTTON,(WPARAM)IDC_AUDIO_CONVERT,(LPARAM)MAKELONG(true,0));
     SendMessage(ToolbarHandle,TB_ENABLEBUTTON,(WPARAM)IDC_AUDIO_SETTINGS,(LPARAM)MAKELONG(true,0));
            
     EnableWindow(ComboBoxHandle,true);
     EnableWindow(ConvertButtonHandle,true);
     EnableWindow(EncoderButtonHandle,true);
     EnableWindow(PauseButtonHandle,false);
     EnableWindow(StopButtonHandle,false);
     
     SendMessage(StatusbarHandle, SB_SETTEXT, 0, (LPARAM)"Audio conversion stopped");
     
     if(on_convert_ontop) SetWindowPos(AudioConverterHandle,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
}

/*****************************************************************************

OnConvert

*****************************************************************************/

void AUDIO_CONVERTER::OnConvert()
{    
     TotalItem=0;
     CurrentItem=0;
     TotalItem=ListView_GetItemCount(ListViewHandle);
     if(TotalItem<=0)
       {
        MessageBox(AudioConverterHandle,"There are no files in the list to convert!!!!","Error!",MB_ICONERROR);
        return;
       }
     else
      {  
       for(int i=0;i<TotalItem;i++)
         ListViewChangeImage(ListViewHandle,i,0);
      
       HMENU hMenu=GetMenu(AudioConverterHandle);
       EnableMenuItem(GetSubMenu(hMenu,0),IDC_AUDIO_ADDFILES,MF_BYCOMMAND|MF_GRAYED);
       EnableMenuItem(GetSubMenu(hMenu,0),IDC_AUDIO_ADDFOLDER,MF_BYCOMMAND|MF_GRAYED);
       EnableMenuItem(GetSubMenu(hMenu,0),IDC_AUDIO_REMOVE,MF_BYCOMMAND|MF_GRAYED);
       EnableMenuItem(GetSubMenu(hMenu,0),IDC_AUDIO_REMOVEALL,MF_BYCOMMAND|MF_GRAYED);
       EnableMenuItem(GetSubMenu(hMenu,2),IDC_AUDIO_CONVERT,MF_BYCOMMAND|MF_GRAYED);
       EnableMenuItem(GetSubMenu(hMenu,2),IDC_AUDIO_SETTINGS,MF_BYCOMMAND|MF_GRAYED);
       EnableMenuItem(GetSubMenu(hMenu,2),IDC_AUDIO_ENCODER,MF_BYCOMMAND|MF_GRAYED);
           
       SendMessage(ToolbarHandle,TB_ENABLEBUTTON,(WPARAM)IDC_AUDIO_CONVERT,(LPARAM)MAKELONG(false,0));
       SendMessage(ToolbarHandle,TB_ENABLEBUTTON,(WPARAM)IDC_AUDIO_ADDFILES,(LPARAM)MAKELONG(false,0));
       SendMessage(ToolbarHandle,TB_ENABLEBUTTON,(WPARAM)IDC_AUDIO_ADDFOLDER,(LPARAM)MAKELONG(false,0));
       SendMessage(ToolbarHandle,TB_ENABLEBUTTON,(WPARAM)IDC_AUDIO_REMOVE,(LPARAM)MAKELONG(false,0));
       SendMessage(ToolbarHandle,TB_ENABLEBUTTON,(WPARAM)IDC_AUDIO_REMOVEALL,(LPARAM)MAKELONG(false,0));
       SendMessage(ToolbarHandle,TB_ENABLEBUTTON,(WPARAM)IDC_AUDIO_CONVERT,(LPARAM)MAKELONG(false,0));
       SendMessage(ToolbarHandle,TB_ENABLEBUTTON,(WPARAM)IDC_AUDIO_SETTINGS,(LPARAM)MAKELONG(false,0));
       
       EnableWindow(ComboBoxHandle,false);
       EnableWindow(ConvertButtonHandle,false);
       EnableWindow(EncoderButtonHandle,false);
       EnableWindow(PauseButtonHandle,true);
       EnableWindow(StopButtonHandle,true);        
       
       convert_exception=true;
       button_exception=true;
       timer_on=true;
       
       TotalPositionInitial=0;
       TotalPositionFinal=0;
       TotalTime=0;
       CurrentTime=0;
       CurrentTotalTime=0;
       Progress1=0;
       Progress2=0;
       
       SendMessage(StatusbarHandle, SB_SETTEXT, 0, (LPARAM)"Analyzing the total time period");
       
       BASS_Init(-1,44100,0,AudioConverterHandle,NULL);
       LoadPlugin();
       
       GetTotalTime(TotalTime,szTotalTime);
      
       SendMessage(StatusbarHandle, SB_SETTEXT, 0, (LPARAM)"Ready for conversion");
       
       //MENU SETTINGS
       
       if(on_convert_ontop) SetWindowPos(AudioConverterHandle,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
       
       SetTimer(AudioConverterHandle,0,0,0);
      }
        
     
    
     
}


/*****************************************************************************

WM_TIMER

*****************************************************************************/

void AUDIO_CONVERTER::OnTimer(HWND hwnd)
{
     char *msg;
     
     if(BASS_ChannelIsActive(nChannel))
       {
        if(BASS_Encode_IsActive(nEncode))
         {
          char temp[20000];
          
		  BASS_ChannelGetData(nChannel,temp,20000);
          
          GetCurrentTime(nChannel,CurrentTime,szCurrentTime);
          GetCurrentTotalTime(nChannel,CurrentTotalTime,szCurrentTotalTime);
          
          TotalPositionFinal=CurrentTime+TotalPositionInitial;
          
          Progress1=Progress2=0;
          
          if(CurrentTotalTime>0 || TotalTime>0)
           {
            Progress1=(CurrentTime*100)/CurrentTotalTime;
            Progress2=(TotalPositionFinal*100)/TotalTime;
           }
          
          SendMessage(ProgressHandle,PBM_SETPOS,Progress1,0);
          SendMessage(OverallProgressHandle,PBM_SETPOS,Progress2,0);
		  
		  ShowTime(TimeHandle1,szCurrentTime,szCurrentTotalTime);
		  ShowTime(TimeHandle2,MakeTime(TotalPositionFinal),szTotalTime);
		  
		  
		  convert_exception=true;
          }
        }
       else if(convert_exception)
        {
          
          convert_exception=false;
          
          int i;
          
          wsprintf(szStatus,"Converting %d file out of %d files",CurrentItem+1,TotalItem);
          SendMessage(StatusbarHandle, SB_SETTEXT, 0, (LPARAM)szStatus);
          
          TotalPositionInitial+=CurrentTime;
          
          ListViewChangeImage(ListViewHandle,CurrentItem-1,1); 
             
          if(nEncode) BASS_Encode_Stop(nEncode);
          
          for(i=CurrentItem;i<TotalItem;i++)
           {
              ListView_SetItemSelect(ListViewHandle,i,true,true);
              nChannel=BASS_MusicLoad(false,GetPathFromListView(ListViewHandle,i),false,false,BASS_MUSIC_DECODE,44100);
              
              if(!nChannel)
                nChannel=BASS_StreamCreateFile(false,GetPathFromListView(ListViewHandle,i),false,false,BASS_STREAM_DECODE);
             
              if(!nChannel)
               {
                  
                ListViewChangeImage(ListViewHandle,i,2);
                
                //Message to display if this is unsupported format............
                //MessageBox(hwnd,"Could not encode this file......","Error!",MB_ICONERROR);
                msg=Registry.ReadStringSetting(AUDIO_CONVERTER_PATH,"dmsg");
                if(!stricmp(msg,"1"))
                 {
                  MessageBox(hwnd,"This file cannot be converted","Error!",MB_ICONERROR);
                 }
                continue;
                
               }
               
              else break;
            }
            
            CurrentItem=i;
            
            /************************************************************************
            
            PLACE TO CHANGE CODE
            
            *************************************************************************/
            if(nChannel)
             {
              szCurrentFileName[0]='\0';
              ListView_GetItemText(ListViewHandle,CurrentItem,0,szCurrentFileName,sizeof szCurrentFileName + 1);
              if(strlen(szCurrentFileName)) GetConverterReference();
              //ListView_SetItemState(ListViewHandle,CurrentItem,LVIS_FOCUSED|LVIS_SELECTED,LVIF_STATE);
             }
            /////////////////////////////////////////////////////////////////////////
            
            if(nChannel && CurrentItem<TotalItem) 
            nEncode=BASS_Encode_Start(nChannel,Conv_Ref,BASS_ENCODE_AUTOFREE,NULL,0);
           
            //if(!nEncode)
               //MessageBox(hwnd,"could not encode","",0);
            
            CurrentItem++;
            
            if(Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"mcolor")==0 ||
               Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"mcolor")==-1)
               AC_ODMenu.SetDefaultMenu();
            
            if(Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"mcolor")==1)
               AC_ODMenu.SetRedMenu();
            
            if(Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"mcolor")==2)
               AC_ODMenu.SetGreenMenu();
               
            if(Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"mcolor")==3)
               AC_ODMenu.SetBlueMenu();
            
         } 
         
         if(button_exception && CurrentItem>TotalItem)
          {
                             
           if(nEncode) BASS_Encode_Stop(nEncode);
           
           HMENU hMenu=GetMenu(hwnd);
           EnableMenuItem(GetSubMenu(hMenu,0),IDC_AUDIO_ADDFILES,MF_BYCOMMAND|MF_ENABLED);
           EnableMenuItem(GetSubMenu(hMenu,0),IDC_AUDIO_ADDFOLDER,MF_BYCOMMAND|MF_ENABLED);
           EnableMenuItem(GetSubMenu(hMenu,0),IDC_AUDIO_REMOVE,MF_BYCOMMAND|MF_ENABLED);
           EnableMenuItem(GetSubMenu(hMenu,0),IDC_AUDIO_REMOVEALL,MF_BYCOMMAND|MF_ENABLED);
           EnableMenuItem(GetSubMenu(hMenu,2),IDC_AUDIO_CONVERT,MF_BYCOMMAND|MF_ENABLED);
           EnableMenuItem(GetSubMenu(hMenu,2),IDC_AUDIO_SETTINGS,MF_BYCOMMAND|MF_ENABLED);
           EnableMenuItem(GetSubMenu(hMenu,2),IDC_AUDIO_ENCODER,MF_BYCOMMAND|MF_ENABLED);
          
           SendMessage(ToolbarHandle,TB_ENABLEBUTTON,(WPARAM)IDC_AUDIO_ADDFILES,(LPARAM)MAKELONG(true,0));
           SendMessage(ToolbarHandle,TB_ENABLEBUTTON,(WPARAM)IDC_AUDIO_ADDFOLDER,(LPARAM)MAKELONG(true,0));
           SendMessage(ToolbarHandle,TB_ENABLEBUTTON,(WPARAM)IDC_AUDIO_REMOVE,(LPARAM)MAKELONG(true,0));
           SendMessage(ToolbarHandle,TB_ENABLEBUTTON,(WPARAM)IDC_AUDIO_REMOVEALL,(LPARAM)MAKELONG(true,0));
           SendMessage(ToolbarHandle,TB_ENABLEBUTTON,(WPARAM)IDC_AUDIO_CONVERT,(LPARAM)MAKELONG(true,0));
           SendMessage(ToolbarHandle,TB_ENABLEBUTTON,(WPARAM)IDC_AUDIO_SETTINGS,(LPARAM)MAKELONG(true,0));
           
           EnableWindow(ComboBoxHandle,true);
           EnableWindow(ConvertButtonHandle,true);
           EnableWindow(EncoderButtonHandle,true);
           EnableWindow(PauseButtonHandle,false);
           EnableWindow(StopButtonHandle,false);
           
           SendMessage(ProgressHandle,PBM_SETPOS,0,0);
           SendMessage(OverallProgressHandle,PBM_SETPOS,0,0);
           SendMessage(TimeHandle1,WM_SETTEXT,0,(LPARAM)"00:00:00 / 00:00:00");
           SendMessage(TimeHandle2,WM_SETTEXT,0,(LPARAM)"00:00:00 / 00:00:00");
           
           SendMessage(StatusbarHandle, SB_SETTEXT, 0, (LPARAM)"RB Audio Converter-Completed");
                 
           //MENU SETTINGS
           if(on_convert_ontop) SetWindowPos(hwnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
           
           msg=Registry.ReadStringSetting(AUDIO_CONVERTER_PATH,"wmsg");
              
              
           button_exception=false;
           
           //Message to post after complition
           msg=Registry.ReadStringSetting(AUDIO_CONVERTER_PATH,"cmsg");
                if(!stricmp(msg,"1"))
                 {
                  MessageBox(hwnd,"File conversion completed.","Information!",MB_ICONINFORMATION);
                 }
           
           if(Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"aconversion")==1) ExitConverter(hwnd);
           //if(Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"aconversion")==2) PostQuitMessage(0);
           if(Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"aconversion")==3) System.LogOff();
           if(Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"aconversion")==4) System.StandBy();
           if(Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"aconversion")==5 && stricmp(msg,"1")) System.Restart();
           if(Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"aconversion")==6 && stricmp(msg,"1")) System.Shutdown();
           
           if(Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"aconversion")==5 && !stricmp(msg,"1"))
            {
             int info=MessageBox(hwnd,"Your have set your options to restart your computer after conversion completes.\nWould you like to restart?","Information",MB_YESNO|MB_ICONINFORMATION);
             if(info==IDYES) System.Restart();
            }
            
            if(Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"aconversion")==6 && !stricmp(msg,"1"))
            {
             int info=MessageBox(hwnd,"Your have set your options to shutdown your computer after conversion completes.\nWould you like to shutdown?","Information",MB_YESNO|MB_ICONINFORMATION);
             if(info==IDYES) System.Shutdown();
            }
           
          }
}


void AUDIO_CONVERTER::GetConverterReference()
{
      int        index;
      char       szOutputPath[MAX_PATH];
      
      index=SendMessage(ComboBoxHandle,CB_GETCURSEL,0,0);
      
      strcpy(szOutputPath,Registry.ReadStringSetting(AUDIO_CONVERTER_PATH,"output"));
      
      if(!strcmp(szOutputPath,"")) strcpy(szOutputPath,General.GetMyMusicDir());
      
      switch(index)
       {
         case 0://aac
              {
               char *sample;
               int mpeg=Registry.ReadNumberSetting(AAC_ENCODER_PATH,"mpeg");
               int noise=Registry.ReadNumberSetting(AAC_ENCODER_PATH,"noise");
               int vbr=Registry.ReadNumberSetting(AAC_ENCODER_PATH,"vbr");
               
               wsprintf(Conv_Ref,"encoders\\aacenc ");
               
               if(mpeg==1)
                 strcat(Conv_Ref,"--mpeg-vers 2 ");
                 
               if(noise==1)
                 strcat(Conv_Ref,"--no-tns ");
                 
               if(vbr==1)
                {
                  strcat(Conv_Ref,"-q ");
                  
                  int vsample=Registry.ReadNumberSetting(AAC_ENCODER_PATH,"vsample");
                  
                  switch(vsample)
                   {
                    case 0:
                         sample="10";
                    break;
                    case 1:
                         sample="50";
                    break;
                    case 2:
                         sample="100";
                    break;
                    case 3:
                         sample="150";
                    break;
                    case 4:
                         sample="200";
                    break;
                    case 5:
                         sample="250";
                    break;
                    case 6:
                         sample="300";
                    break;
                    case 7:
                         sample="350";
                    break;
                    case 8:
                         sample="400";
                    break;
                    case 9:
                         sample="450";
                    break;
                    case 10:
                         sample="500";
                    break;
                    default:
                         sample="100";
                            
                   }
                }
               else
                {
                  strcat(Conv_Ref,"-b "); 
                  
                  int asample=Registry.ReadNumberSetting(AAC_ENCODER_PATH,"asample");
                  
                  switch(asample)
                   {
                     case 0:
                         sample="16";
                    break;
                    case 1:
                         sample="24";
                    break;
                    case 2:
                         sample="32";
                    break;
                    case 3:
                         sample="40";
                    break;
                    case 4:
                         sample="48";
                    break;
                    case 5:
                         sample="56";
                    break;
                    case 6:
                         sample="64";
                    break;
                    case 7:
                         sample="80";
                    break;
                    case 8:
                         sample="96";
                    break;
                    case 9:
                         sample="112";
                    break;
                    case 10:
                         sample="128";
                    break;
                    case 11:
                         sample="144";
                    break;
                    case 12:
                         sample="160";
                    break;
                    case 13:
                         sample="192";
                    break;
                    case 14:
                         sample="224";
                    break;
                    case 15:
                         sample="256";
                    break;
                    case 16:
                         sample="320";
                    break;
                    default:
                         sample="128";
                   }
                }
               
               strcat(Conv_Ref,sample);
               strcat(Conv_Ref," -o \"");
               strcat(Conv_Ref,szOutputPath);
               strcat(Conv_Ref,"\\");
               strcat(Conv_Ref,szCurrentFileName);
               strcat(Conv_Ref,".aac\" -");
               
               wsprintf(szActualFile,"%hs\\%hs.aac",szOutputPath,szCurrentFileName);
              }
         break;
         case 1://flac
              {
               int channel=Registry.ReadNumberSetting(FLAC_ENCODER_PATH,"channel");
               int sample=Registry.ReadNumberSetting(FLAC_ENCODER_PATH,"sample");
               int compression=Registry.ReadNumberSetting(FLAC_ENCODER_PATH,"compression");
               
               //CHANNEL
               
               switch(channel)
               {
                 case 0:
                      channel=1;
                 break;
                 case 1:
                      channel=2;
                 break;
                 default:
                      channel=2;   
               }
               
               //SAMPLE
               
               switch(sample)
               {
                case 0:
                     sample=8000;
                break;
                case 1:
                     sample=16000;
                break;
                case 2:
                     sample=22050;
                break;
                case 3:
                     sample=32000;
                break;
                case 4:
                     sample=44100;
                break;
                case 5:
                     sample=48000;
                break;
                case 6:
                     sample=96000;
                break;
                default:
                     sample=44100;
               }
               
               switch(compression)
               {
                case 0:
                     compression=0;
                break;
                case 1:
                     compression=1;
                break;
                case 2:
                     compression=2;
                break;
                case 3:
                     compression=3;
                break;
                case 4:
                     compression=4;
                break;
                case 5:
                     compression=5;
                break;
                case 6:
                     compression=6;
                break;
                case 7:
                     compression=7;
                break;
                case 8:
                     compression=8;
                break;
                default:
                     compression=4;
               }
               wsprintf(Conv_Ref,"encoders\\flacenc -%d --channels=%d --sample-rate=%d -o \"%hs\\%hs.flac\" -",
                        compression,channel,sample,
                        szOutputPath,
                        szCurrentFileName);
               
               wsprintf(szActualFile,"%hs\\%hs.flac",szOutputPath,szCurrentFileName);
               
              }
         break;
         
         case 2://mp3
              {
               int temp;

               strcpy(Conv_Ref,"encoders\\mp3enc ");
                 
               temp=Registry.ReadNumberSetting(MP3_ENCODER_PATH,"error");
               if(temp) strcat(Conv_Ref,"-p ");
                 
               temp=Registry.ReadNumberSetting(MP3_ENCODER_PATH,"channel");
               
               switch(temp)
                {
                 case 0:
                      strcat(Conv_Ref,"-m s ");
                 break;
                 case 1:
                      strcat(Conv_Ref,"-m j ");
                 break;
                 case 2:
                      strcat(Conv_Ref,"-m f ");
                 break;
                 case 3:
                      strcat(Conv_Ref,"-m m ");
                 break;
                 default:
                      strcat(Conv_Ref,"-m j ");
                }
                
               temp=Registry.ReadNumberSetting(MP3_ENCODER_PATH,"cbr");
               
               if(temp==1 || temp==-1)
                {
                
                 temp=Registry.ReadNumberSetting(MP3_ENCODER_PATH,"quality");
                 if(temp) strcat(Conv_Ref,"-h ");
                 
                 temp=Registry.ReadNumberSetting(MP3_ENCODER_PATH,"cbitrate");
                 
                 switch(temp)
                  {
                   case 0:
                        strcat(Conv_Ref,"-b 8 ");
                   break;
                   case 1:
                        strcat(Conv_Ref,"-b 16 ");
                   break;
                   case 2:
                        strcat(Conv_Ref,"-b 24 ");
                   break;
                   case 3:
                        strcat(Conv_Ref,"-b 32 ");
                   break;
                   case 4:
                        strcat(Conv_Ref,"-b 40 ");
                   break;
                   case 5:
                        strcat(Conv_Ref,"-b 48 ");
                   break;
                   case 6:
                        strcat(Conv_Ref,"-b 56 ");
                   break;
                   case 7:
                        strcat(Conv_Ref,"-b 64 ");
                   break;
                   case 8:
                        strcat(Conv_Ref,"-b 80 ");
                   break;
                   case 9:
                        strcat(Conv_Ref,"-b 96 ");
                   break;
                   case 10:
                        strcat(Conv_Ref,"-b 112 ");
                   break;
                   case 11:
                        strcat(Conv_Ref,"-b 128 ");
                   break;
                   case 12:
                        strcat(Conv_Ref,"-b 160 ");
                   break;
                   case 13:
                        strcat(Conv_Ref,"-b 192 ");
                   break;
                   case 14:
                        strcat(Conv_Ref,"-b 224 ");
                   break;
                   case 15:
                        strcat(Conv_Ref,"-b 256 ");
                   break;
                   case 16:
                        strcat(Conv_Ref,"-b 320 ");
                   break;
                   default:
                        strcat(Conv_Ref,"-b 128 ");   
                  }
                  
                }
               else
                {
                 strcat(Conv_Ref,"-v ");
                 
                 temp=Registry.ReadNumberSetting(MP3_ENCODER_PATH,"vquality");
                 
                 switch(temp)
                  {
                   case 0:
                        strcat(Conv_Ref,"-V 0 ");
                   break;
                   case 1:
                        strcat(Conv_Ref,"-V 1 ");
                   break;
                   case 2:
                        strcat(Conv_Ref,"-V 2 ");
                   break;
                   case 3:
                        strcat(Conv_Ref,"-V 3 ");
                   break;
                   case 4:
                        strcat(Conv_Ref,"-V 4 ");
                   break;
                   case 5:
                        strcat(Conv_Ref,"-V 5 ");
                   break; 
                   case 6:
                        strcat(Conv_Ref,"-V 6 ");
                   break;
                   case 7:
                        strcat(Conv_Ref,"-V 7 ");
                   break;
                   case 8:
                        strcat(Conv_Ref,"-V 8 ");
                   break;
                   case 9:
                        strcat(Conv_Ref,"-V 9 ");
                   break;    
                   default:
                        strcat(Conv_Ref,"-V 4 ");
                  }
                 
                 temp=Registry.ReadNumberSetting(MP3_ENCODER_PATH,"vminbitrate");
                 
                 switch(temp)
                 {
                   case 0:
                        strcat(Conv_Ref,"-b 8 ");
                   break;
                   case 1:
                        strcat(Conv_Ref,"-b 16 ");
                   break;
                   case 2:
                        strcat(Conv_Ref,"-b 24 ");
                   break;
                   case 3:
                        strcat(Conv_Ref,"-b 32 ");
                   break;
                   case 4:
                        strcat(Conv_Ref,"-b 40 ");
                   break;
                   case 5:
                        strcat(Conv_Ref,"-b 48 ");
                   break;
                   case 6:
                        strcat(Conv_Ref,"-b 56 ");
                   break;
                   case 7:
                        strcat(Conv_Ref,"-b 64 ");
                   break;
                   case 8:
                        strcat(Conv_Ref,"-b 80 ");
                   break;
                   case 9:
                        strcat(Conv_Ref,"-b 96 ");
                   break;
                   case 10:
                        strcat(Conv_Ref,"-b 112 ");
                   break;
                   case 11:
                        strcat(Conv_Ref,"-b 128 ");
                   break;
                   case 12:
                        strcat(Conv_Ref,"-b 160 ");
                   break;
                   case 13:
                        strcat(Conv_Ref,"-b 192 ");
                   break;
                   case 14:
                        strcat(Conv_Ref,"-b 224 ");
                   break;
                   case 15:
                        strcat(Conv_Ref,"-b 256 ");
                   break;
                   case 16:
                        strcat(Conv_Ref,"-b 320 ");
                   break;
                   default:
                        strcat(Conv_Ref,"-b 32 ");   
                }
                  
                temp=Registry.ReadNumberSetting(MP3_ENCODER_PATH,"vmaxbitrate");
                 
                 switch(temp)
                 {
                   case 0:
                        strcat(Conv_Ref,"-B 8 ");
                   break;
                   case 1:
                        strcat(Conv_Ref,"-B 16 ");
                   break;
                   case 2:
                        strcat(Conv_Ref,"-B 24 ");
                   break;
                   case 3:
                        strcat(Conv_Ref,"-B 32 ");
                   break;
                   case 4:
                        strcat(Conv_Ref,"-B 40 ");
                   break;
                   case 5:
                        strcat(Conv_Ref,"-B 48 ");
                   break;
                   case 6:
                        strcat(Conv_Ref,"-B 56 ");
                   break;
                   case 7:
                        strcat(Conv_Ref,"-B 64 ");
                   break;
                   case 8:
                        strcat(Conv_Ref,"-B 80 ");
                   break;
                   case 9:
                        strcat(Conv_Ref,"-B 96 ");
                   break;
                   case 10:
                        strcat(Conv_Ref,"-B 112 ");
                   break;
                   case 11:
                        strcat(Conv_Ref,"-B 128 ");
                   break;
                   case 12:
                        strcat(Conv_Ref,"-B 160 ");
                   break;
                   case 13:
                        strcat(Conv_Ref,"-B 192 ");
                   break;
                   case 14:
                        strcat(Conv_Ref,"-B 224 ");
                   break;
                   case 15:
                        strcat(Conv_Ref,"-B 256 ");
                   break;
                   case 16:
                        strcat(Conv_Ref,"-B 320 ");
                   break;
                   default:
                        strcat(Conv_Ref,"-B 1128 ");   
                }
                
              }
              strcat(Conv_Ref,"- \"");
              strcat(Conv_Ref,szOutputPath);
              strcat(Conv_Ref,"\\");
              strcat(Conv_Ref,szCurrentFileName);
              strcat(Conv_Ref,".mp3\""); 
              }
         break;
         
         case 3://mpc
              {
               char temp[20];
               int quality=Registry.ReadNumberSetting(MPC_ENCODER_PATH,"quality");
               
               switch(quality)
                {
                  case 0:
                       strcpy(temp,"thumb");
                  break;
                  case 1:
                       strcpy(temp,"radio");
                  break;
                  case 2:
                       strcpy(temp,"standard");
                  break;
                  case 3:
                       strcpy(temp,"extreme");
                  break;
                  case 4:
                       strcpy(temp,"insane");
                  break;
                  default:
                       strcpy(temp,"standard");   
                }
                wsprintf(Conv_Ref,"encoders\\mpcenc --%hs - \"%hs\\%hs.mpc\"",
                        temp,
                        szOutputPath,
                        szCurrentFileName);
                
                wsprintf(szActualFile,"%hs\\%hs.mpc",szOutputPath,szCurrentFileName);
              }
         break;
       }
       
       
      
}

/*****************************************************************************

GetTotalTime

*****************************************************************************/

void AUDIO_CONVERTER::GetTotalTime(int &nTime,char *szTime)
{
      int index;
      int timecount=0;
      char chtime[20];
      int hr,min,sec;
      char temp[256];
      
      index=ListView_GetItemCount(ListViewHandle);
      
      for(int i=0;i<=index;i++)
       {
        HSTREAM chan=BASS_StreamCreateFile(false,GetPathFromListView(ListViewHandle,i),false,false,BASS_STREAM_DECODE);
        if(!chan) continue;
        timecount+=(int)BASS_ChannelBytes2Seconds(chan,BASS_ChannelGetLength(chan,BASS_POS_BYTE));
        BASS_StreamFree(chan);
        wsprintf(temp,"Analyzing the total time period ( %d",(i*100)/index);
        strcat(temp,"% )");
        SendMessage(StatusbarHandle, SB_SETTEXT, 0, (LPARAM)temp);
       }
       nTime=timecount;
       
       SplitTime(nTime,hr,min,sec);
       wsprintf(chtime,"%.2d:%.2d:%.2d",hr,min,sec);
       
       strcpy(szTime,chtime);
}



/*****************************************************************************

GetCurrentTotalTime

*****************************************************************************/

void AUDIO_CONVERTER::GetCurrentTotalTime(HSTREAM chan,int &nTime,char *szTime)
{
      int timecount=0;
      char chtime[20];
      int hr,min,sec;
      
       timecount=(int)BASS_ChannelBytes2Seconds(chan,BASS_ChannelGetLength(chan,BASS_POS_BYTE));
       
       nTime=timecount;
       
       SplitTime(nTime,hr,min,sec);
       
       wsprintf(chtime,"%.2d:%.2d:%.2d",hr,min,sec);
       
       strcpy(szTime,chtime);
}



/*****************************************************************************

GetCurrentTime

*****************************************************************************/

void AUDIO_CONVERTER::GetCurrentTime(HSTREAM chan,int &nTime,char *szTime)
{
      int timecount=0;
      char chtime[20];
      int hr,min,sec;
       
       timecount=(int)BASS_ChannelBytes2Seconds(chan,BASS_ChannelGetPosition(chan,BASS_POS_BYTE));
       
       nTime=timecount;
       
       SplitTime(nTime,hr,min,sec);
       
       wsprintf(chtime,"%.2d:%.2d:%.2d",hr,min,sec);
       
       strcpy(szTime,chtime);
}


/*****************************************************************************

ConcatTime

*****************************************************************************/


void AUDIO_CONVERTER::ShowTime(HWND hwnd,char* szTime1,char* szTime2)
{
      static char temp[30];
      char temp1[30];
      
      strcpy(temp,szTime1);
      strcat(temp," / ");
      strcat(temp,szTime2);
      
      GetWindowText(hwnd,temp1,sizeof temp1 + 1);
      
      if(strcmp(temp1,temp))
       SendMessage(hwnd,WM_SETTEXT,0,(LPARAM)temp);
}

char* AUDIO_CONVERTER::MakeTime(int time)
{
      int hr,min,sec;
      static char temp[30];
      
      SplitTime(time,hr,min,sec);
      
      wsprintf(temp,"%.2d:%.2d:%.2d",hr,min,sec);
      
      return temp;
}

void AUDIO_CONVERTER::SplitTime(int timecount,int &hour, int &minute, int &second)
{
      int hr,min,sec;
      
      hr=min=sec=0;
      
      while(timecount>59)
      {
        timecount-=60;
        min++;
        
        if(min==60)
         {
          min=0;
          hr++;
         }
      }
      sec=timecount;
      
      hour=hr;
      minute=min;
      second=sec;
}

/*****************************************************************************

WM_SIZE

*****************************************************************************/

void AUDIO_CONVERTER::OnSize(HWND hwnd,WPARAM wParam,LPARAM lParam)
{
   
  SetWindowPos(ToolbarHandle,NULL,0,0,LOWORD(lParam)-10,HIWORD(lParam)-200,SWP_NOMOVE);
  SetWindowPos(ListViewHandle,NULL,0,0,LOWORD(lParam)-10,HIWORD(lParam)-300,SWP_NOMOVE);
  SendMessage(ToolbarHandle,TB_SETBUTTONWIDTH,0,MAKELONG(LOWORD(lParam)/9-4,LOWORD(lParam)/9-4));
  
  SetWindowPos(ProgressGroupHandle,NULL,5,HIWORD(lParam)-205,LOWORD(lParam)-170,130,0);
  
  
  SetWindowPos(StaticHandle1,NULL,15,HIWORD(lParam)-170,0,0,SWP_NOSIZE);
  SetWindowPos(ProgressHandle,NULL,85,HIWORD(lParam)-171,LOWORD(lParam)-370,20,0);
  SetWindowPos(TimeHandle1,NULL,LOWORD(lParam)-283,HIWORD(lParam)-170,0,0,SWP_NOSIZE);
  
  SetWindowPos(StaticHandle2,NULL,15,HIWORD(lParam)-120,0,0,SWP_NOSIZE);
  SetWindowPos(OverallProgressHandle,NULL,85,HIWORD(lParam)-122,LOWORD(lParam)-370,20,0);
  SetWindowPos(TimeHandle2,NULL,LOWORD(lParam)-283,HIWORD(lParam)-120,0,0,SWP_NOSIZE);
  
  SetWindowPos(FormatGroupHandle,NULL,LOWORD(lParam)-155,HIWORD(lParam)-205,0,0,SWP_NOSIZE);
  SetWindowPos(StaticHandle3,NULL,LOWORD(lParam)-140,HIWORD(lParam)-178,0,0,SWP_NOSIZE);
  SetWindowPos(ComboBoxHandle,NULL,LOWORD(lParam)-140,HIWORD(lParam)-155,0,0,SWP_NOSIZE);
  SetWindowPos(EncoderButtonHandle,NULL,LOWORD(lParam)-150,HIWORD(lParam)-118,0,0,SWP_NOSIZE);
  
  SetWindowPos(ConvertButtonHandle,NULL,10,HIWORD(lParam)-73,0,0,SWP_NOSIZE);
  SetWindowPos(PauseButtonHandle,NULL,(LOWORD(lParam)/2)-80,HIWORD(lParam)-73,0,0,SWP_NOSIZE);
  SetWindowPos(StopButtonHandle,NULL,LOWORD(lParam)-180,HIWORD(lParam)-73,0,0,SWP_NOSIZE);
  
  SendMessage(StatusbarHandle, WM_SIZE, 0, 0);
}


/*****************************************************************************

WM_DROPFILES

*****************************************************************************/

void AUDIO_CONVERTER::OnDropFile(HWND hwnd,WPARAM wParam)
{
     WIN32_FIND_DATA   wfd;
     HANDLE            hFile;
     char              szPath[MAX_PATH+1];
     UINT              count;
     int               i;
     DWORD             nSize;
     bool              filter=false;
     bool              filter_dialog=false;
     char              szTemp[21];
     
     count=DragQueryFile((HDROP)wParam,0xFFFFFFFF,NULL,0);
     
     for(i=0;i<count;i++)
     {
       DragQueryFile((HDROP)wParam,i,szPath,sizeof(szPath)+1);
       
       hFile=FindFirstFile(szPath,&wfd);
       
       if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
         {
         filter_dialog=true;
         break;
         }
     }
     
     if(filter_dialog) 
        DialogBox(NULL,MAKEINTRESOURCE(IDD_AUDIO_FILTER),hwnd,(DLGPROC)AudioFilter);
     
     General.StripLeading(szEditText);
     
     if(!strcmp(szEditText,"*.*")) strcpy(szEditText,".");
     else if(szEditText[0]=='*')
       {
        strrev(szEditText);
        szEditText[strlen(szEditText)-1]='\0';
        strrev(szEditText);
       }
     
     
     for(i=0;i<count;i++)
     {
        DragQueryFile((HDROP)wParam,i,szPath,sizeof(szPath)+1);
      
        hFile=FindFirstFile(szPath,&wfd);
      
        if(hFile==INVALID_HANDLE_VALUE) continue;
      
        HCURSOR hCursor=SetCursor(LoadCursor(NULL,IDC_WAIT));
        
        if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
           FindFile_Recursive(szPath,szEditText);
        else
           SetPathToList(szPath);
        
        SetCursor(hCursor);
         
      
     }
FindClose(hFile);
strcpy(szEditText,"");
}


/*****************************************************************************

AUDIO_EXIT

*****************************************************************************/


void AUDIO_CONVERTER::ExitConverter(HWND hwnd)
{
   if(timer_on) KillTimer(hwnd,0);
   ImageList_Destroy(ToolbarImageList);
   ImageList_Destroy(ListViewImageList);
   ImageList_Destroy(ConvertImageList);
   ImageList_Destroy(PauseImageList);
   ImageList_Destroy(StopImageList);
   DeleteObject(StaticFont1);
   DeleteObject(StaticFont2);
   DeleteObject(StaticFont3);
   
   TotalItem=0;
   CurrentItem=0;
   KillTimer(hwnd,0);
   BASS_Encode_Stop(nEncode);
   BASS_StreamFree(nChannel);  
   BASS_Free();
    
   StoreSettings();
   DestroyWindow(hwnd);
}

/*****************************************************************************

OnRemoveAll

*****************************************************************************/


void AUDIO_CONVERTER::OnRemoveAll()
{
     if(ListView_GetItemCount(ListViewHandle)<=0)
      {
       MessageBox(AudioConverterHandle,"There are no files added to the list ?","Information",MB_ICONEXCLAMATION | MB_OK);
       return;
      }
      
     if(MessageBox(AudioConverterHandle,"Are you sure you want to remove all files ?","Information",MB_ICONEXCLAMATION|MB_YESNO)==IDYES)
     ListView_DeleteAllItems(ListViewHandle);
     
     wsprintf(szStatus,"%d Files",ListView_GetItemCount(ListViewHandle));
     SendMessage(StatusbarHandle,SB_SETTEXT,1,(LPARAM)szStatus);
}


/*****************************************************************************

OnRemove

*****************************************************************************/

void AUDIO_CONVERTER::OnRemove()
{
     if(ListView_GetItemCount(ListViewHandle)<=0)
       {
        MessageBox(AudioConverterHandle,"There are no files added to the list ?","Information",MB_ICONEXCLAMATION | MB_OK);
        return;
       }
     
     int index=ListView_GetNextItem(ListViewHandle,(WPARAM)-1,LVNI_FOCUSED|LVNI_SELECTED);
     
     if(index!=-1)
       {
          ListView_DeleteItem(ListViewHandle,index);
          
          wsprintf(szStatus,"%d Files",ListView_GetItemCount(ListViewHandle));
          SendMessage(StatusbarHandle,SB_SETTEXT,1,(LPARAM)szStatus);
       } 
     else
        MessageBox(AudioConverterHandle,"No item selected to remove ?","Information",MB_ICONEXCLAMATION | MB_OK);
}


/*****************************************************************************

EXPLORE FOLDER

*****************************************************************************/

void AUDIO_CONVERTER::OnExplore()
{
     char *temp;
     
     temp=Registry.ReadStringSetting(AUDIO_CONVERTER_PATH,"output");
     
     if(!strcmp(temp,""))
       ShellExecuteA(NULL,"open",General.GetMyMusicDir(),NULL,NULL,SW_SHOWNORMAL);
     else
       ShellExecuteA(NULL,"open",temp,NULL,NULL,SW_SHOWNORMAL);
}


void AUDIO_CONVERTER::OnProperties()
{
     if(ListView_GetItemCount(ListViewHandle)<=0) return;
     
     int index=ListView_GetNextItem(ListViewHandle,(WPARAM)-1,LVNI_FOCUSED);
     
     if(index!=-1)
       Shell.SHProperties(GetPathFromListView(ListViewHandle,index));
     else
       return;
}

void AUDIO_CONVERTER::FolderExplore()
{
     if(ListView_GetItemCount(ListViewHandle)<=0) return;
     
     int index=ListView_GetNextItem(ListViewHandle,(WPARAM)-1,LVNI_FOCUSED);
     
    // MessageBox(0,"","",0);
     
     if(index!=-1)
       ShellExecute(NULL,"explore",GetListViewColumnText(ListViewHandle,index,3),NULL,NULL,SW_SHOW);
     else
       return;
}
/*****************************************************************************

ADD FILES

*****************************************************************************/

void AUDIO_CONVERTER::AddFiles()
{
     char temp[MAX_PATH + 1];
     
     strcpy(temp,Shell.FileSelectDialog(AudioConverterHandle,"All Files(*.*)\0*.*\0\0"));
    
     if(strlen(temp)!=0)
       {
        HCURSOR hCursor=SetCursor(LoadCursor(NULL,IDC_WAIT));
        SetPathToList(temp);
        SetCursor(hCursor);
       } 
}


/*****************************************************************************

ADD FOLDER

*****************************************************************************/

void AUDIO_CONVERTER::AddFolder()
{
     char *temp;
     temp=Shell.BrowseForFolder(AudioConverterHandle);
     
     for(int i='a';i<='z';i++)
      {
       char temp1[20];
       
       wsprintf(temp1,"%c:\\",i);
       UINT type=GetDriveType(temp1);
       
         switch(type)
          {
            case DRIVE_CDROM:
            case DRIVE_FIXED:
            case DRIVE_REMOVABLE:
            case DRIVE_REMOTE:
            case DRIVE_RAMDISK:
                 if(!stricmp(temp1,temp))
                    temp[strlen(temp)-1]='\0';
            break;
          }
      }
     
     if(strlen(temp)==0) return;
     
     DialogBox(NULL,MAKEINTRESOURCE(IDD_AUDIO_FILTER),AudioConverterHandle,(DLGPROC)AudioFilter);
     
     if(!strcmp(szEditText,"")) return;
     
     General.StripLeading(szEditText);
     
     if(!strcmp(szEditText,"*.*")) strcpy(szEditText,".");
     else if(szEditText[0]=='*')
       {
        strrev(szEditText);
        szEditText[strlen(szEditText)-1]='\0';
        strrev(szEditText);
       }
     HCURSOR hCursor=SetCursor(LoadCursor(NULL,IDC_WAIT));
     FindFile_Recursive(temp,szEditText);
     SetCursor(hCursor);
}

/*****************************************************************************

ADDMENU

*****************************************************************************/

void AUDIO_CONVERTER::AddMenu(HWND hWnd)
{
    HMENU hMenu=LoadMenu(GetModuleHandle(NULL),MAKEINTRESOURCE(IDM_AUDIO_CONVERTER));
    SetMenu(hWnd,hMenu);
    MenuHandle=hMenu;
    int temp;
    
    HMENU hViewMenu=GetSubMenu(hMenu,1);
    HMENU hConvertMenu=GetSubMenu(hMenu,2);
    
    HMENU hOnTop=GetSubMenu(hViewMenu,0);
    HMENU hTrans=GetSubMenu(hViewMenu,1);
    
    HMENU hColor=GetSubMenu(hViewMenu,3);
    HMENU hMain=GetSubMenu(hColor,0);
    HMENU hPopup=GetSubMenu(hColor,1);
    
    HMENU hConversion=GetSubMenu(hConvertMenu,5);
    
    
    //CODE TO PLACE FOR MENU BACKGROUND CHANGE
    
    if(Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"mcolor")==0 ||
       Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"mcolor")==-1)
       {
        CheckMenuRadioItem(hMain,0,4,0,MF_BYPOSITION);
        AC_ODMenu.SetDefaultMenu();
       }
    
    if(Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"mcolor")==1)
       {
        CheckMenuRadioItem(hMain,0,4,2,MF_BYPOSITION);
        AC_ODMenu.SetRedMenu();
       }
    
    if(Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"mcolor")==2)
       {
        CheckMenuRadioItem(hMain,0,4,3,MF_BYPOSITION);
        AC_ODMenu.SetGreenMenu();
       }
    
    if(Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"mcolor")==3)
       {
        CheckMenuRadioItem(hMain,0,4,4,MF_BYPOSITION);
        AC_ODMenu.SetBlueMenu();
       }
    
    
    
    if(Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"pcolor")==0 ||
       Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"pcolor")==-1)
       {
        CheckMenuRadioItem(hPopup,0,4,0,MF_BYPOSITION);
       }
    
    if(Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"pcolor")==1)
       {
        CheckMenuRadioItem(hPopup,0,4,2,MF_BYPOSITION);
       }
    
    if(Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"pcolor")==2)
       {
        CheckMenuRadioItem(hPopup,0,4,3,MF_BYPOSITION);
       }
    
    if(Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"pcolor")==3)
       {
        CheckMenuRadioItem(hPopup,0,4,4,MF_BYPOSITION);
       }
       
    
    //////////////////////////////////////////////////////////////////////////
    
    ///////////////////////////////////////////////////////////////////////////
    //SYSTEM MENU
    
    HMENU sysmenu=GetSystemMenu(hWnd,false);
    int   cnt=GetMenuItemCount(sysmenu);
    
    AC_ODMenu.ConvertMenu(sysmenu,true);
    
    for(int i=0;i<cnt;i++)
     {
      
      UINT id=GetMenuItemID(sysmenu,i);
      
       switch(id)
        {
         case SC_CLOSE:
              AC_ODMenu.AddMenuIcon(NULL,27);
         break;
         default:
             AC_ODMenu.AddMenuIcon(NULL,50); 
        }
        
     }
    //////////////////////////////////////////////////////////////////////////
    
    ///////////////////////////////////////////////////////////////////////////
    //FILE MENU
    
    AC_ODMenu.ConvertMenu(GetSubMenu(hMenu,0),true);
    AC_ODMenu.AddMenuIcon(LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_TOOLBAR_ADDFILES)),0);
    AC_ODMenu.AddMenuIcon(LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_TOOLBAR_ADDFOLDER)),0);
    AC_ODMenu.AddMenuIcon(LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_TOOLBAR_OUTPUTFOLDER)),0);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_TOOLBAR_REMOVE)),0);
    AC_ODMenu.AddMenuIcon(LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_TOOLBAR_REMOVE)),0);
    AC_ODMenu.AddMenuIcon(NULL,50);
    //AC_ODMenu.AddMenuIcon(NULL,134);
    AC_ODMenu.AddMenuIcon(NULL,19);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_TOOLBAR_EXIT)),0);
    
    ////////////////////////////////////////////////////////////////////////
    
    
    
    ///////////////////////////////////////////////////////////////////////
    //VIEW MENU
    
    AC_ODMenu.ConvertMenu(GetSubMenu(hMenu,1),true);
    AC_ODMenu.AddMenuIcon(NULL,24);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(NULL,50);
    
    //ON TOP
    AC_ODMenu.ConvertMenu(GetSubMenu(GetSubMenu(hMenu,1),0),true);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(NULL,50);
    
    
    //TRANSPARENCY
    AC_ODMenu.ConvertMenu(GetSubMenu(GetSubMenu(hMenu,1),1),true);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(NULL,50);
    
    
    //MENU COLOR
    AC_ODMenu.ConvertMenu(GetSubMenu(GetSubMenu(hMenu,1),3),true);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(NULL,50);
    
    
    //MAIN MENU
    AC_ODMenu.ConvertMenu(GetSubMenu(GetSubMenu(GetSubMenu(hMenu,1),3),0),true);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(NULL,50);
    
    
    //POPUP MENU
    AC_ODMenu.ConvertMenu(GetSubMenu(GetSubMenu(GetSubMenu(hMenu,1),3),1),true);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(NULL,50);
    
    
    ///////////////////////////////////////////////////////////////////////
    
    
    
    
    ///////////////////////////////////////////////////////////////////////
    //CONVERT MENU
    
    AC_ODMenu.ConvertMenu(GetSubMenu(hMenu,2),true);
    
    AC_ODMenu.AddMenuIcon(LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_TOOLBAR_CONVERT)),0);
    AC_ODMenu.AddMenuIcon(LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_TOOLBAR_SETTINGS)),0);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(NULL,40);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_TOOLBAR_CONVERT)),0);
    
    //AFTER CONVERSION
    AC_ODMenu.ConvertMenu(GetSubMenu(GetSubMenu(hMenu,2),5),true);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(NULL,50);
    //AC_ODMenu.AddMenuIcon(NULL,50);
    ///////////////////////////////////////////////////////////////////////////
    
    
    
    
    ///////////////////////////////////////////////////////////////////////////
    //HELP MENU
    
    AC_ODMenu.ConvertMenu(GetSubMenu(hMenu,3),true);
    AC_ODMenu.AddMenuIcon(LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_TOOLBAR_HELP)),0);
    AC_ODMenu.AddMenuIcon(NULL,50);
    AC_ODMenu.AddMenuIcon(NULL,23);
    
    ////////////////////////////////////////////////////////////////////////////
    
    //INTIALIZE SETTINGS...................................
    
    temp=Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"ontop");
   
    if(temp==-1 || temp==0)
     {
      CheckMenuRadioItem(hOnTop,0,3,0,MF_BYPOSITION);
      on_convert_ontop=false;
     }
     
    if(temp==1)
     {
       CheckMenuRadioItem(hOnTop,0,3,2,MF_BYPOSITION);
       SetWindowPos(hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
       on_convert_ontop=false;
     }
    if(temp==2)
     {
       CheckMenuRadioItem(hOnTop,0,3,3,MF_BYPOSITION);
       on_convert_ontop=true;
     }
     
     temp=Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"transparency");
     
     if(temp==-1 || temp==0)
      {
       CheckMenuRadioItem(hTrans,0,11,0,MF_BYPOSITION);
      }
      
       if(temp==1)
      {
       CheckMenuRadioItem(hTrans,0,11,2,MF_BYPOSITION);
       System.SetTransparency(hWnd,10);
      }
      
      if(temp==2)
      {
       CheckMenuRadioItem(hTrans,0,11,3,MF_BYPOSITION);
       System.SetTransparency(hWnd,20);
      }
      
       if(temp==3)
      {
       CheckMenuRadioItem(hTrans,0,11,4,MF_BYPOSITION);
       System.SetTransparency(hWnd,30);
      }
      
       if(temp==4)
      {
       CheckMenuRadioItem(hTrans,0,11,5,MF_BYPOSITION);
       System.SetTransparency(hWnd,40);
      }
      
       if(temp==5)
      {
       CheckMenuRadioItem(hTrans,0,11,6,MF_BYPOSITION);
       System.SetTransparency(hWnd,50);
      }
      
       if(temp==6)
      {
       CheckMenuRadioItem(hTrans,0,11,7,MF_BYPOSITION);
       System.SetTransparency(hWnd,60);
      }
      
       if(temp==7)
      {
       CheckMenuRadioItem(hTrans,0,11,8,MF_BYPOSITION);
       System.SetTransparency(hWnd,70);
      }
      
       if(temp==8)
      {
       CheckMenuRadioItem(hTrans,0,11,9,MF_BYPOSITION);
       System.SetTransparency(hWnd,80);
      }
      
       if(temp==9)
      {
       CheckMenuRadioItem(hTrans,0,11,10,MF_BYPOSITION);
       System.SetTransparency(hWnd,90);
      }
      
      // AFTER CONVERSION........................................................
      
      if(Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"aconversion")==0 ||
         Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"aconversion")==-1)
         CheckMenuRadioItem(hConversion,0,6,0,MF_BYPOSITION);
         
      if(Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"aconversion")==1)
         CheckMenuRadioItem(hConversion,0,6,2,MF_BYPOSITION);
      
      //if(Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"aconversion")==2)
      //   CheckMenuRadioItem(hConversion,0,6,3,MF_BYPOSITION);
         
      if(Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"aconversion")==3)
         CheckMenuRadioItem(hConversion,0,6,3,MF_BYPOSITION);
         
      if(Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"aconversion")==4)
         CheckMenuRadioItem(hConversion,0,6,4,MF_BYPOSITION);
      
      if(Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"aconversion")==5)
         CheckMenuRadioItem(hConversion,0,6,5,MF_BYPOSITION);
      
      if(Registry.ReadNumberSetting(AUDIO_CONVERTER_PATH,"aconversion")==6)
         CheckMenuRadioItem(hConversion,0,6,6,MF_BYPOSITION);
}


/*****************************************************************************

AUDIO FILTER

*****************************************************************************/


LRESULT CALLBACK AUDIO_CONVERTER::AudioFilter(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
        switch(msg)
        {
                   case WM_INITDIALOG:
                        {
                         General.CenterWindow(AudioConverter.AudioConverterHandle,hwnd);
                         SendMessage(GetDlgItem(hwnd,IDC_AUDIOFILTER_EDIT),EM_LIMITTEXT,(WPARAM)20,(LPARAM)0);
                         SendMessage(GetDlgItem(hwnd,IDC_AUDIOFILTER_EDIT),WM_SETTEXT,0,(LPARAM)"*.*");
                         
                         SetFocus(GetDlgItem(hwnd,IDC_AUDIOFILTER_OK));
                        }
                   break;
                   case WM_COMMAND:
                        switch(LOWORD(wParam))
                        {
                          case IDC_AUDIOFILTER_CANCEL:
                               {
                               strcpy(AudioConverter.szEditText,"");
                               EndDialog(hwnd,true);
                               }
                          break;
                          case IDC_AUDIOFILTER_OK:
                               {
                               GetWindowText(GetDlgItem(hwnd,IDC_AUDIOFILTER_EDIT),AudioConverter.szEditText,21);
                               EndDialog(hwnd,true);
                               }
                          break;
                        }
                   break;
                   case WM_CLOSE:
                        {
                        strcpy(AudioConverter.szEditText,"");
                        EndDialog(hwnd,true);
                        }
                   break;
        }
return false;
}




/***********************************************************************************


                                  ENCODERS
                                  

************************************************************************************/


/*****************************************************************************

Choose Correct Encoder

*****************************************************************************/

void AUDIO_CONVERTER::GetEncoderDialog(HWND hwnd)
{
     int index=SendMessage(ComboBoxHandle,CB_GETCURSEL,0,0);
     
     switch(index)
      {
        case 0:
            DialogBox(NULL,MAKEINTRESOURCE(IDD_ENCODER_AAC),hwnd,(DLGPROC)Aac);
        break;
        case 1:
            DialogBox(NULL,MAKEINTRESOURCE(IDD_ENCODER_FLAC),hwnd,(DLGPROC)Flac);
        break;
        case 2:
            DialogBox(NULL,MAKEINTRESOURCE(IDD_ENCODER_MP3),hwnd,(DLGPROC)Mp3);
        break;
        case 3:
             DialogBox(NULL,MAKEINTRESOURCE(IDD_ENCODER_MPC),hwnd,(DLGPROC)Mpc);
        break;
        case 4:
             DialogBox(NULL,MAKEINTRESOURCE(IDD_ENCODER_OGG),hwnd,(DLGPROC)Ogg);
        break;
        case 5:
             DialogBox(NULL,MAKEINTRESOURCE(IDD_ENCODER_SPX),hwnd,(DLGPROC)Spx);
        break;
             
      }
}






/*****************************************************************************

AAC ENCODER

*****************************************************************************/

LRESULT CALLBACK AUDIO_CONVERTER::Aac(HWND hwnd, UINT msg,WPARAM wParam, LPARAM lParam)
{
        switch(msg)
        {
         case WM_INITDIALOG:
             {
              General.CenterWindow(AudioConverter.AudioConverterHandle,hwnd);
           
              HWND hABR=GetDlgItem(hwnd,IDC_ENC_AAC_ABR);
              HWND hVBR=GetDlgItem(hwnd,IDC_ENC_AAC_VBR);
                             
              SendMessage(hABR,CB_ADDSTRING,0,(LPARAM)"16");
              SendMessage(hABR,CB_ADDSTRING,1,(LPARAM)"24");
              SendMessage(hABR,CB_ADDSTRING,2,(LPARAM)"32");
              SendMessage(hABR,CB_ADDSTRING,3,(LPARAM)"40");
              SendMessage(hABR,CB_ADDSTRING,4,(LPARAM)"48");
              SendMessage(hABR,CB_ADDSTRING,5,(LPARAM)"56");
              SendMessage(hABR,CB_ADDSTRING,6,(LPARAM)"64");
              SendMessage(hABR,CB_ADDSTRING,7,(LPARAM)"80");
              SendMessage(hABR,CB_ADDSTRING,8,(LPARAM)"96");
              SendMessage(hABR,CB_ADDSTRING,9,(LPARAM)"112");
              SendMessage(hABR,CB_ADDSTRING,10,(LPARAM)"128");
              SendMessage(hABR,CB_ADDSTRING,11,(LPARAM)"144");
              SendMessage(hABR,CB_ADDSTRING,12,(LPARAM)"160");
              SendMessage(hABR,CB_ADDSTRING,13,(LPARAM)"192");
              SendMessage(hABR,CB_ADDSTRING,14,(LPARAM)"224");
              SendMessage(hABR,CB_ADDSTRING,15,(LPARAM)"256");
              SendMessage(hABR,CB_ADDSTRING,16,(LPARAM)"320");
              
              SendMessage(hVBR,CB_ADDSTRING,0,(LPARAM)"10");
              SendMessage(hVBR,CB_ADDSTRING,1,(LPARAM)"50");
              SendMessage(hVBR,CB_ADDSTRING,2,(LPARAM)"100");
              SendMessage(hVBR,CB_ADDSTRING,3,(LPARAM)"150");
              SendMessage(hVBR,CB_ADDSTRING,4,(LPARAM)"200");
              SendMessage(hVBR,CB_ADDSTRING,5,(LPARAM)"250");
              SendMessage(hVBR,CB_ADDSTRING,6,(LPARAM)"300");
              SendMessage(hVBR,CB_ADDSTRING,7,(LPARAM)"350");
              SendMessage(hVBR,CB_ADDSTRING,8,(LPARAM)"400");
              SendMessage(hVBR,CB_ADDSTRING,9,(LPARAM)"450");
              SendMessage(hVBR,CB_ADDSTRING,10,(LPARAM)"500");
              
              AudioConverter.InitializeSettings_aac(hwnd);
              
              SetFocus(GetDlgItem(hwnd,IDC_ENC_AAC_OK));
             }
           
         break;
         case WM_COMMAND:
              switch(LOWORD(wParam))
              {
                case IDC_ENC_AAC_OK:
                     AudioConverter.StoreSettings_aac(hwnd);
                     EndDialog(hwnd,true);
                break;
                
                case IDC_ENC_AAC_CANCEL:
                     EndDialog(hwnd,true);
                break;
                
                case IDC_ENC_AAC_RESTORE:
                     AudioConverter.aac_default(hwnd);
                break;
                
                case IDC_ENC_AAC_RADIO1:
                     {
                     EnableWindow(GetDlgItem(hwnd,IDC_ENC_AAC_VBR),false);
                     EnableWindow(GetDlgItem(hwnd,IDC_ENC_AAC_ABR),true);
                     }
                break;
                
                case IDC_ENC_AAC_RADIO2:
                     {
                     EnableWindow(GetDlgItem(hwnd,IDC_ENC_AAC_VBR),true);
                     EnableWindow(GetDlgItem(hwnd,IDC_ENC_AAC_ABR),false);
                     }
                break;
                
                case IDC_ENC_AAC_CHECK1:
                    
                break;
                
                case IDC_ENC_AAC_CHECK2:
                break;
              }
         break;
         case WM_CLOSE:
           EndDialog(hwnd,true);
         break;
        }
return FALSE;
}


void AUDIO_CONVERTER::aac_default(HWND hwnd)
{
    HWND hABR=GetDlgItem(hwnd,IDC_ENC_AAC_ABR);
    HWND hVBR=GetDlgItem(hwnd,IDC_ENC_AAC_VBR);
    HWND mpeg=GetDlgItem(hwnd,IDC_ENC_AAC_CHECK1);
    HWND noise=GetDlgItem(hwnd,IDC_ENC_AAC_CHECK2);
    
    EnableWindow(hABR,false);
    EnableWindow(hVBR,true);
    SendMessage(GetDlgItem(hwnd,IDC_ENC_AAC_RADIO2),BM_SETCHECK,BST_CHECKED,0);
    SendMessage(GetDlgItem(hwnd,IDC_ENC_AAC_RADIO1),BM_SETCHECK,BST_UNCHECKED,0);
    SendMessage(mpeg,BM_SETCHECK,BST_UNCHECKED,0);
    SendMessage(noise,BM_SETCHECK,BST_UNCHECKED,0);
    
    SendMessage(hVBR,CB_SETCURSEL,2,0);
    SendMessage(hABR,CB_SETCURSEL,10,0);
    
}


void AUDIO_CONVERTER::InitializeSettings_aac(HWND hwnd)
{
    int temp;
    HWND hABR=GetDlgItem(hwnd,IDC_ENC_AAC_ABR);
    HWND hVBR=GetDlgItem(hwnd,IDC_ENC_AAC_VBR);
    HWND mpeg=GetDlgItem(hwnd,IDC_ENC_AAC_CHECK1);
    HWND noise=GetDlgItem(hwnd,IDC_ENC_AAC_CHECK2);
    
    temp=Registry.ReadNumberSetting(AAC_ENCODER_PATH,"vbr");
    
    if(temp==1 || temp==-1)
     {
      temp=1;
      Registry.WriteNumberSetting(AAC_ENCODER_PATH,"vbr",temp);
      Registry.WriteNumberSetting(AAC_ENCODER_PATH,"abr",0);
      EnableWindow(hABR,false);
      EnableWindow(hVBR,true);
      SendMessage(GetDlgItem(hwnd,IDC_ENC_AAC_RADIO2),BM_SETCHECK,BST_CHECKED,0);
     }
    else
     {
      Registry.WriteNumberSetting(AAC_ENCODER_PATH,"vbr",0);
      Registry.WriteNumberSetting(AAC_ENCODER_PATH,"abr",1);
      EnableWindow(hABR,true);
      EnableWindow(hVBR,false);
      SendMessage(GetDlgItem(hwnd,IDC_ENC_AAC_RADIO1),BM_SETCHECK,BST_CHECKED,0);
     }
    
    temp=Registry.ReadNumberSetting(AAC_ENCODER_PATH,"mpeg");
    
    if(temp==-1 || temp==0)
      {
        temp=0;
        Registry.WriteNumberSetting(AAC_ENCODER_PATH,"mpeg",0);
      }
     else
      {
        Registry.WriteNumberSetting(AAC_ENCODER_PATH,"mpeg",1);
        SendMessage(GetDlgItem(hwnd,IDC_ENC_AAC_CHECK1),BM_SETCHECK,BST_CHECKED,0);
      }
      
    temp=Registry.ReadNumberSetting(AAC_ENCODER_PATH,"noise");
    
    if(temp==-1 || temp==0)
      {
        temp=0;
        Registry.WriteNumberSetting(AAC_ENCODER_PATH,"noise",0);
      }
     else
      {
        Registry.WriteNumberSetting(AAC_ENCODER_PATH,"noise",1);
        SendMessage(GetDlgItem(hwnd,IDC_ENC_AAC_CHECK2),BM_SETCHECK,BST_CHECKED,0);
      }
    
    temp=Registry.ReadNumberSetting(AAC_ENCODER_PATH,"asample");
    
    if(temp==-1)
     {
      Registry.WriteNumberSetting(AAC_ENCODER_PATH,"asample",10);
      SendMessage(hABR,CB_SETCURSEL,10,0);
     }
    else
     {
      SendMessage(hABR,CB_SETCURSEL,temp,0);
     }
     
     temp=Registry.ReadNumberSetting(AAC_ENCODER_PATH,"vsample");
    
    if(temp==-1)
     {
      Registry.WriteNumberSetting(AAC_ENCODER_PATH,"vsample",2);
      SendMessage(hVBR,CB_SETCURSEL,2,0);
     }
    else
     {
      SendMessage(hVBR,CB_SETCURSEL,temp,0);
     }
}

void AUDIO_CONVERTER::StoreSettings_aac(HWND hwnd)
{
    int temp;
    HWND hABR=GetDlgItem(hwnd,IDC_ENC_AAC_ABR);
    HWND hVBR=GetDlgItem(hwnd,IDC_ENC_AAC_VBR);
    HWND mpeg=GetDlgItem(hwnd,IDC_ENC_AAC_CHECK1);
    HWND noise=GetDlgItem(hwnd,IDC_ENC_AAC_CHECK2);
    HWND radio1=GetDlgItem(hwnd,IDC_ENC_AAC_RADIO1);
    HWND check1=GetDlgItem(hwnd,IDC_ENC_AAC_CHECK1);
    HWND check2=GetDlgItem(hwnd,IDC_ENC_AAC_CHECK2);
    
    Registry.WriteNumberSetting(AAC_ENCODER_PATH,"asample",SendMessage(hABR,CB_GETCURSEL,0,0));
    Registry.WriteNumberSetting(AAC_ENCODER_PATH,"vsample",SendMessage(hVBR,CB_GETCURSEL,0,0));
    
    temp=SendMessage(radio1,BM_GETCHECK,0,0);
    
    if(temp==BST_CHECKED)
     {
      Registry.WriteNumberSetting(AAC_ENCODER_PATH,"vbr",0);
      Registry.WriteNumberSetting(AAC_ENCODER_PATH,"abr",1);
     }
    else
     {
       Registry.WriteNumberSetting(AAC_ENCODER_PATH,"vbr",1);
       Registry.WriteNumberSetting(AAC_ENCODER_PATH,"abr",0);
     }
     
     temp=SendMessage(check1,BM_GETCHECK,0,0);
     
     if(temp==BST_CHECKED)
       Registry.WriteNumberSetting(AAC_ENCODER_PATH,"mpeg",1);
     else
       Registry.WriteNumberSetting(AAC_ENCODER_PATH,"mpeg",0);
       
     temp=SendMessage(check2,BM_GETCHECK,0,0);
     
     if(temp==BST_CHECKED)
       Registry.WriteNumberSetting(AAC_ENCODER_PATH,"noise",1);
     else
       Registry.WriteNumberSetting(AAC_ENCODER_PATH,"noise",0);
}






/*****************************************************************************

FLAC ENCODER

*****************************************************************************/

LRESULT CALLBACK AUDIO_CONVERTER::Flac(HWND hwnd, UINT msg,WPARAM wParam, LPARAM lParam)
{
        switch(msg)
        {
                       case WM_INITDIALOG:
                            {
                             General.CenterWindow(AudioConverter.AudioConverterHandle,hwnd);
                             HWND hCompress=GetDlgItem(hwnd,IDC_ENC_FLAC_COMPRESS);
                             HWND hSample=GetDlgItem(hwnd,IDC_ENC_FLAC_SAMPLE);
                             HWND hChannel=GetDlgItem(hwnd,IDC_ENC_FLAC_CHANNEL);
                             
                             SendMessage(hCompress,CB_ADDSTRING,0,(LPARAM)"0(fastest)");
                             SendMessage(hCompress,CB_ADDSTRING,1,(LPARAM)"1");
                             SendMessage(hCompress,CB_ADDSTRING,2,(LPARAM)"2");
                             SendMessage(hCompress,CB_ADDSTRING,3,(LPARAM)"3");
                             SendMessage(hCompress,CB_ADDSTRING,4,(LPARAM)"4");
                             SendMessage(hCompress,CB_ADDSTRING,5,(LPARAM)"5");
                             SendMessage(hCompress,CB_ADDSTRING,6,(LPARAM)"6");
                             SendMessage(hCompress,CB_ADDSTRING,7,(LPARAM)"7");
                             SendMessage(hCompress,CB_ADDSTRING,8,(LPARAM)"8(best)");
                             
                             SendMessage(hSample,CB_ADDSTRING,0,(LPARAM)"8000");
                             SendMessage(hSample,CB_ADDSTRING,1,(LPARAM)"16000");
                             SendMessage(hSample,CB_ADDSTRING,2,(LPARAM)"22050");
                             SendMessage(hSample,CB_ADDSTRING,3,(LPARAM)"32000");
                             SendMessage(hSample,CB_ADDSTRING,4,(LPARAM)"44100");
                             SendMessage(hSample,CB_ADDSTRING,5,(LPARAM)"48000");
                             SendMessage(hSample,CB_ADDSTRING,6,(LPARAM)"96000");
                             
                             SendMessage(hChannel,CB_ADDSTRING,6,(LPARAM)"Mono");
                             SendMessage(hChannel,CB_ADDSTRING,6,(LPARAM)"Stereo");
                             
                             AudioConverter.InitializeSettings_Flac(hwnd);
                             
                             SetFocus(GetDlgItem(hwnd,IDC_ENC_FLAC_OK));
                            }
                       break;
                       case WM_COMMAND:
                            switch(LOWORD(wParam))
                             {
                               
                               case IDC_ENC_FLAC_RESTORE:
                                    AudioConverter.flac_default(hwnd);
                               break;
                               case IDC_ENC_FLAC_CANCEL:
                                    EndDialog(hwnd,true);
                               break;
                               case IDC_ENC_FLAC_OK:
                                    AudioConverter.StoreSettings_Flac(hwnd);
                                    EndDialog(hwnd,true);
                               break;
                             }
                        break;        
                       case WM_CLOSE:
                            EndDialog(hwnd,true);
                       break;
        }
return false;
}



void AUDIO_CONVERTER::flac_default(HWND hwnd)
{
     HWND hCompress=GetDlgItem(hwnd,IDC_ENC_FLAC_COMPRESS);
     HWND hSample=GetDlgItem(hwnd,IDC_ENC_FLAC_SAMPLE);
     HWND hChannel=GetDlgItem(hwnd,IDC_ENC_FLAC_CHANNEL);
     
     SendMessage(hCompress,CB_SETCURSEL,4,0);
     SendMessage(hSample,CB_SETCURSEL,4,0);
     SendMessage(hChannel,CB_SETCURSEL,1,0);
}



void AUDIO_CONVERTER::InitializeSettings_Flac(HWND hwnd)
{
     int temp;
     HWND hCompress=GetDlgItem(hwnd,IDC_ENC_FLAC_COMPRESS);
     HWND hSample=GetDlgItem(hwnd,IDC_ENC_FLAC_SAMPLE);
     HWND hChannel=GetDlgItem(hwnd,IDC_ENC_FLAC_CHANNEL);
     
     temp=Registry.ReadNumberSetting(FLAC_ENCODER_PATH,"channel");
     
     if(temp==-1) temp=1;
     Registry.WriteNumberSetting(FLAC_ENCODER_PATH,"channel",temp);
     SendMessage(hChannel,CB_SETCURSEL,temp,0);
     
     
     temp=Registry.ReadNumberSetting(FLAC_ENCODER_PATH,"sample");
     
     if(temp==-1) temp=4;
     Registry.WriteNumberSetting(FLAC_ENCODER_PATH,"sample",temp);
     SendMessage(hSample,CB_SETCURSEL,temp,0);
     
     temp=Registry.ReadNumberSetting(FLAC_ENCODER_PATH,"compression");
     
     if(temp==-1) temp=4;
     Registry.WriteNumberSetting(FLAC_ENCODER_PATH,"compression",temp);
     SendMessage(hCompress,CB_SETCURSEL,temp,0);
     
     
}



void AUDIO_CONVERTER::StoreSettings_Flac(HWND hwnd)
{
     HWND hCompress=GetDlgItem(hwnd,IDC_ENC_FLAC_COMPRESS);
     HWND hSample=GetDlgItem(hwnd,IDC_ENC_FLAC_SAMPLE);
     HWND hChannel=GetDlgItem(hwnd,IDC_ENC_FLAC_CHANNEL);
     
     Registry.WriteNumberSetting(FLAC_ENCODER_PATH,"channel",SendMessage(hChannel,CB_GETCURSEL,0,0));
     Registry.WriteNumberSetting(FLAC_ENCODER_PATH,"sample",SendMessage(hSample,CB_GETCURSEL,0,0));
     Registry.WriteNumberSetting(FLAC_ENCODER_PATH,"compression",SendMessage(hCompress,CB_GETCURSEL,0,0));
}



/*****************************************************************************

MP3 ENCODER

*****************************************************************************/

LRESULT CALLBACK AUDIO_CONVERTER::Mp3(HWND hwnd, UINT msg,WPARAM wParam, LPARAM lParam)
{
        switch(msg)
        {
         case WM_INITDIALOG:
             {
              General.CenterWindow(AudioConverter.AudioConverterHandle,hwnd);   
           
              HWND hCBR=GetDlgItem(hwnd,IDC_ENC_MP3_CBR);
              HWND hVBR_Min=GetDlgItem(hwnd,IDC_ENC_MP3_VBR_MIN);
              HWND hVBR_Max=GetDlgItem(hwnd,IDC_ENC_MP3_VBR_MAX);
              HWND hVBR_Quality=GetDlgItem(hwnd,IDC_ENC_MP3_CQUALITY);
              HWND Channel=GetDlgItem(hwnd,IDC_ENC_MP3_CHANNEL);
              
              SendMessage(hCBR,CB_ADDSTRING,0,(LPARAM)"8");
              SendMessage(hCBR,CB_ADDSTRING,1,(LPARAM)"16");
              SendMessage(hCBR,CB_ADDSTRING,2,(LPARAM)"24");
              SendMessage(hCBR,CB_ADDSTRING,3,(LPARAM)"32");
              SendMessage(hCBR,CB_ADDSTRING,4,(LPARAM)"40");
              SendMessage(hCBR,CB_ADDSTRING,5,(LPARAM)"48");
              SendMessage(hCBR,CB_ADDSTRING,6,(LPARAM)"56");
              SendMessage(hCBR,CB_ADDSTRING,7,(LPARAM)"64");
              SendMessage(hCBR,CB_ADDSTRING,8,(LPARAM)"80");
              SendMessage(hCBR,CB_ADDSTRING,9,(LPARAM)"96");
              SendMessage(hCBR,CB_ADDSTRING,10,(LPARAM)"112");
              SendMessage(hCBR,CB_ADDSTRING,11,(LPARAM)"128");
              SendMessage(hCBR,CB_ADDSTRING,12,(LPARAM)"160");
              SendMessage(hCBR,CB_ADDSTRING,13,(LPARAM)"192");
              SendMessage(hCBR,CB_ADDSTRING,14,(LPARAM)"224");
              SendMessage(hCBR,CB_ADDSTRING,15,(LPARAM)"256");
              SendMessage(hCBR,CB_ADDSTRING,16,(LPARAM)"320");
              
              SendMessage(hVBR_Min,CB_ADDSTRING,0,(LPARAM)"8");
              SendMessage(hVBR_Min,CB_ADDSTRING,1,(LPARAM)"16");
              SendMessage(hVBR_Min,CB_ADDSTRING,2,(LPARAM)"24");
              SendMessage(hVBR_Min,CB_ADDSTRING,3,(LPARAM)"32");
              SendMessage(hVBR_Min,CB_ADDSTRING,4,(LPARAM)"40");
              SendMessage(hVBR_Min,CB_ADDSTRING,5,(LPARAM)"48");
              SendMessage(hVBR_Min,CB_ADDSTRING,6,(LPARAM)"56");
              SendMessage(hVBR_Min,CB_ADDSTRING,7,(LPARAM)"64");
              SendMessage(hVBR_Min,CB_ADDSTRING,8,(LPARAM)"80");
              SendMessage(hVBR_Min,CB_ADDSTRING,9,(LPARAM)"96");
              SendMessage(hVBR_Min,CB_ADDSTRING,10,(LPARAM)"112");
              SendMessage(hVBR_Min,CB_ADDSTRING,11,(LPARAM)"128");
              SendMessage(hVBR_Min,CB_ADDSTRING,12,(LPARAM)"160");
              SendMessage(hVBR_Min,CB_ADDSTRING,13,(LPARAM)"192");
              SendMessage(hVBR_Min,CB_ADDSTRING,14,(LPARAM)"224");
              SendMessage(hVBR_Min,CB_ADDSTRING,15,(LPARAM)"256");
              SendMessage(hVBR_Min,CB_ADDSTRING,16,(LPARAM)"320");
              
              SendMessage(hVBR_Max,CB_ADDSTRING,0,(LPARAM)"8");
              SendMessage(hVBR_Max,CB_ADDSTRING,1,(LPARAM)"16");
              SendMessage(hVBR_Max,CB_ADDSTRING,2,(LPARAM)"24");
              SendMessage(hVBR_Max,CB_ADDSTRING,3,(LPARAM)"32");
              SendMessage(hVBR_Max,CB_ADDSTRING,4,(LPARAM)"40");
              SendMessage(hVBR_Max,CB_ADDSTRING,5,(LPARAM)"48");
              SendMessage(hVBR_Max,CB_ADDSTRING,6,(LPARAM)"56");
              SendMessage(hVBR_Max,CB_ADDSTRING,7,(LPARAM)"64");
              SendMessage(hVBR_Max,CB_ADDSTRING,8,(LPARAM)"80");
              SendMessage(hVBR_Max,CB_ADDSTRING,9,(LPARAM)"96");
              SendMessage(hVBR_Max,CB_ADDSTRING,10,(LPARAM)"112");
              SendMessage(hVBR_Max,CB_ADDSTRING,11,(LPARAM)"128");
              SendMessage(hVBR_Max,CB_ADDSTRING,12,(LPARAM)"160");
              SendMessage(hVBR_Max,CB_ADDSTRING,13,(LPARAM)"192");
              SendMessage(hVBR_Max,CB_ADDSTRING,14,(LPARAM)"224");
              SendMessage(hVBR_Max,CB_ADDSTRING,15,(LPARAM)"256");
              SendMessage(hVBR_Max,CB_ADDSTRING,16,(LPARAM)"320");
              
              SendMessage(hVBR_Quality,CB_ADDSTRING,0,(LPARAM)"0 (Highest)");
              SendMessage(hVBR_Quality,CB_ADDSTRING,1,(LPARAM)"1");
              SendMessage(hVBR_Quality,CB_ADDSTRING,2,(LPARAM)"2");
              SendMessage(hVBR_Quality,CB_ADDSTRING,3,(LPARAM)"3");
              SendMessage(hVBR_Quality,CB_ADDSTRING,4,(LPARAM)"4");
              SendMessage(hVBR_Quality,CB_ADDSTRING,5,(LPARAM)"5");
              SendMessage(hVBR_Quality,CB_ADDSTRING,6,(LPARAM)"6");
              SendMessage(hVBR_Quality,CB_ADDSTRING,7,(LPARAM)"7");
              SendMessage(hVBR_Quality,CB_ADDSTRING,8,(LPARAM)"8");
              SendMessage(hVBR_Quality,CB_ADDSTRING,9,(LPARAM)"9 (Lowest)");
              
              SendMessage(Channel,CB_ADDSTRING,0,(LPARAM)"Stereo");
              SendMessage(Channel,CB_ADDSTRING,0,(LPARAM)"Joint");
              SendMessage(Channel,CB_ADDSTRING,0,(LPARAM)"Force");
              SendMessage(Channel,CB_ADDSTRING,0,(LPARAM)"Mono");
              
              AudioConverter.InitializeSettings_mp3(hwnd);
             }
          
         break;
         case WM_COMMAND:
              switch(LOWORD(wParam))
               {
                case IDC_ENC_MP3_RESTORE:
                     {
                      AudioConverter.mp3_default(hwnd);
                     }
                break;
                case IDC_ENC_MP3_OK:
                     {
                      AudioConverter.StoreSettings_mp3(hwnd);
                      EndDialog(hwnd,true);
                     }
                break;
                case IDC_ENC_MP3_RADIO1:
                     {
                      HWND hCBR=GetDlgItem(hwnd,IDC_ENC_MP3_CBR);
                      HWND hVBR_Min=GetDlgItem(hwnd,IDC_ENC_MP3_VBR_MIN);
                      HWND hVBR_Max=GetDlgItem(hwnd,IDC_ENC_MP3_VBR_MAX);
                      HWND hVBR_Quality=GetDlgItem(hwnd,IDC_ENC_MP3_CQUALITY);
                      HWND Quality=GetDlgItem(hwnd,IDC_ENC_MP3_QUALITY);
                      
                      EnableWindow(hCBR,true);
                      EnableWindow(hVBR_Min,false);
                      EnableWindow(hVBR_Max,false);
                      EnableWindow(hVBR_Quality,false);
                      EnableWindow(Quality,true);
                     }
                break;
                case IDC_ENC_MP3_RADIO2:
                     {
                      HWND hCBR=GetDlgItem(hwnd,IDC_ENC_MP3_CBR);
                      HWND hVBR_Min=GetDlgItem(hwnd,IDC_ENC_MP3_VBR_MIN);
                      HWND hVBR_Max=GetDlgItem(hwnd,IDC_ENC_MP3_VBR_MAX);
                      HWND hVBR_Quality=GetDlgItem(hwnd,IDC_ENC_MP3_CQUALITY);                   
                      HWND Quality=GetDlgItem(hwnd,IDC_ENC_MP3_QUALITY);
                      
                      EnableWindow(hCBR,false);
                      EnableWindow(hVBR_Min,true);
                      EnableWindow(hVBR_Max,true);
                      EnableWindow(hVBR_Quality,true);
                      EnableWindow(Quality,false);
                     }
                break;
                case IDC_ENC_MP3_CANCEL:
                     {
                      EndDialog(hwnd,true);
                     }
                break;
               }
         break;
         case WM_CLOSE:
           EndDialog(hwnd,true);
         break;
        }
return FALSE;
}


void AUDIO_CONVERTER::mp3_default(HWND hwnd)
{
     HWND hCBR=GetDlgItem(hwnd,IDC_ENC_MP3_CBR);
     HWND hVBR_Min=GetDlgItem(hwnd,IDC_ENC_MP3_VBR_MIN);
     HWND hVBR_Max=GetDlgItem(hwnd,IDC_ENC_MP3_VBR_MAX);
     HWND hVBR_Quality=GetDlgItem(hwnd,IDC_ENC_MP3_CQUALITY);
     HWND hCBR_Radio=GetDlgItem(hwnd,IDC_ENC_MP3_RADIO1);
     HWND hVBR_Radio=GetDlgItem(hwnd,IDC_ENC_MP3_RADIO2);
     HWND Quality=GetDlgItem(hwnd,IDC_ENC_MP3_QUALITY);
     HWND Channel=GetDlgItem(hwnd,IDC_ENC_MP3_CHANNEL);
     HWND Error=GetDlgItem(hwnd,IDC_ENC_MP3_ERROR);
     
     SendMessage(hCBR_Radio,BM_SETCHECK,BST_CHECKED,0);
     EnableWindow(hCBR,true);
     EnableWindow(Quality,true);
     SendMessage(hCBR,CB_SETCURSEL,11,0);
     SendMessage(Quality,BM_SETCHECK,BST_CHECKED,0);
     
     
     SendMessage(hVBR_Radio,BM_SETCHECK,BST_UNCHECKED,0);
     SendMessage(hVBR_Min,CB_SETCURSEL,3,0);
     EnableWindow(hVBR_Min,false);
     SendMessage(hVBR_Max,CB_SETCURSEL,11,0);
     EnableWindow(hVBR_Max,false);
     SendMessage(hVBR_Quality,CB_SETCURSEL,4,0);
     EnableWindow(hVBR_Quality,false);
     
     SendMessage(Channel,CB_SETCURSEL,1,0);
     SendMessage(Error,BM_SETCHECK,BST_UNCHECKED,0);
     
}


void AUDIO_CONVERTER::InitializeSettings_mp3(HWND hwnd)
{
     HWND hCBR=GetDlgItem(hwnd,IDC_ENC_MP3_CBR);
     HWND hVBR_Min=GetDlgItem(hwnd,IDC_ENC_MP3_VBR_MIN);
     HWND hVBR_Max=GetDlgItem(hwnd,IDC_ENC_MP3_VBR_MAX);
     HWND hVBR_Quality=GetDlgItem(hwnd,IDC_ENC_MP3_CQUALITY);
     HWND hCBR_Radio=GetDlgItem(hwnd,IDC_ENC_MP3_RADIO1);
     HWND hVBR_Radio=GetDlgItem(hwnd,IDC_ENC_MP3_RADIO2);
     HWND Quality=GetDlgItem(hwnd,IDC_ENC_MP3_QUALITY);
     HWND Channel=GetDlgItem(hwnd,IDC_ENC_MP3_CHANNEL);
     HWND Error=GetDlgItem(hwnd,IDC_ENC_MP3_ERROR);
     
     int temp;
     
     temp=Registry.ReadNumberSetting(MP3_ENCODER_PATH,"cbr");
     
     if(temp==1 || temp==-1)
      {
       SendMessage(hCBR_Radio,BM_SETCHECK,BST_CHECKED,0);
       SendMessage(hVBR_Radio,BM_SETCHECK,BST_UNCHECKED,0);
       EnableWindow(hCBR,true);
       EnableWindow(hVBR_Min,false);
       EnableWindow(hVBR_Max,false);
       EnableWindow(hVBR_Quality,false);
      }
     else
      {
       SendMessage(hCBR_Radio,BM_SETCHECK,BST_UNCHECKED,0);
       SendMessage(hVBR_Radio,BM_SETCHECK,BST_CHECKED,0);
       EnableWindow(hCBR,false);
       EnableWindow(hVBR_Min,true);
       EnableWindow(hVBR_Max,true);
       EnableWindow(hVBR_Quality,true);
      }
      
      temp=Registry.ReadNumberSetting(MP3_ENCODER_PATH,"cbitrate");
      
      if(temp==-1)
        SendMessage(hCBR,CB_SETCURSEL,11,0);
      else
        SendMessage(hCBR,CB_SETCURSEL,temp,0);
        
      
       temp=Registry.ReadNumberSetting(MP3_ENCODER_PATH,"vminbitrate");
       
       if(temp==-1)
         SendMessage(hVBR_Min,CB_SETCURSEL,3,0);
       else
          SendMessage(hVBR_Min,CB_SETCURSEL,temp,0);
          
       
       temp=Registry.ReadNumberSetting(MP3_ENCODER_PATH,"vmaxbitrate");
       
       if(temp==-1)
         SendMessage(hVBR_Max,CB_SETCURSEL,11,0);
       else
          SendMessage(hVBR_Max,CB_SETCURSEL,temp,0);
      
      temp=Registry.ReadNumberSetting(MP3_ENCODER_PATH,"vquality");
      
      if(temp==-1)
        SendMessage(hVBR_Quality,CB_SETCURSEL,4,0);
      else
        SendMessage(hVBR_Quality,CB_SETCURSEL,temp,0);
        
      
      temp=Registry.ReadNumberSetting(MP3_ENCODER_PATH,"channel");
      
      if(temp==-1)
        SendMessage(Channel,CB_SETCURSEL,1,0);
      else
        SendMessage(Channel,CB_SETCURSEL,temp,0);
        
      temp=Registry.ReadNumberSetting(MP3_ENCODER_PATH,"quality");
      
      if(temp==-1 || temp==1)
        SendMessage(Quality,BM_SETCHECK,BST_CHECKED,0);
      else
        SendMessage(Quality,BM_SETCHECK,BST_UNCHECKED,0);
        
      temp=Registry.ReadNumberSetting(MP3_ENCODER_PATH,"error");
      
      if(temp==-1 || temp==0)
        SendMessage(Error,BM_SETCHECK,BST_UNCHECKED,0);
      else
        SendMessage(Error,BM_SETCHECK,BST_CHECKED,0);
}

void AUDIO_CONVERTER::StoreSettings_mp3(HWND hwnd)
{
     HWND hCBR=GetDlgItem(hwnd,IDC_ENC_MP3_CBR);
     HWND hVBR_Min=GetDlgItem(hwnd,IDC_ENC_MP3_VBR_MIN);
     HWND hVBR_Max=GetDlgItem(hwnd,IDC_ENC_MP3_VBR_MAX);
     HWND hVBR_Quality=GetDlgItem(hwnd,IDC_ENC_MP3_CQUALITY);
     HWND hCBR_Radio=GetDlgItem(hwnd,IDC_ENC_MP3_RADIO1);
     HWND hVBR_Radio=GetDlgItem(hwnd,IDC_ENC_MP3_RADIO2);
     HWND Quality=GetDlgItem(hwnd,IDC_ENC_MP3_QUALITY);
     HWND Channel=GetDlgItem(hwnd,IDC_ENC_MP3_CHANNEL);
     HWND Error=GetDlgItem(hwnd,IDC_ENC_MP3_ERROR);
     
     Registry.WriteNumberSetting(MP3_ENCODER_PATH,"cbitrate",SendMessage(hCBR,CB_GETCURSEL,0,0));
     Registry.WriteNumberSetting(MP3_ENCODER_PATH,"vminbitrate",SendMessage(hVBR_Min,CB_GETCURSEL,0,0));
     Registry.WriteNumberSetting(MP3_ENCODER_PATH,"vmaxbitrate",SendMessage(hVBR_Max,CB_GETCURSEL,0,0));
     Registry.WriteNumberSetting(MP3_ENCODER_PATH,"vquality",SendMessage(hVBR_Quality,CB_GETCURSEL,0,0));
     Registry.WriteNumberSetting(MP3_ENCODER_PATH,"vminbitrate",SendMessage(hVBR_Min,CB_GETCURSEL,0,0));
     Registry.WriteNumberSetting(MP3_ENCODER_PATH,"channel",SendMessage(Channel,CB_GETCURSEL,0,0));
     
     
      if(SendMessage(Quality,BM_GETCHECK,0,0)==BST_CHECKED)
       {
        Registry.WriteNumberSetting(MP3_ENCODER_PATH,"quality",1);
       }
      else
       {
        Registry.WriteNumberSetting(MP3_ENCODER_PATH,"quality",0);
       } 
       
     if(SendMessage(hCBR_Radio,BM_GETCHECK,0,0)==BST_CHECKED)
      {
       Registry.WriteNumberSetting(MP3_ENCODER_PATH,"cbr",1);
       Registry.WriteNumberSetting(MP3_ENCODER_PATH,"vbr",0);
      }
     else
      {
       Registry.WriteNumberSetting(MP3_ENCODER_PATH,"cbr",0);
       Registry.WriteNumberSetting(MP3_ENCODER_PATH,"vbr",1);
      }
     
     if(SendMessage(Error,BM_GETCHECK,0,0)==BST_CHECKED)
       {
        Registry.WriteNumberSetting(MP3_ENCODER_PATH,"error",1);
       }
      else
       {
        Registry.WriteNumberSetting(MP3_ENCODER_PATH,"error",0);
       } 
}





/*****************************************************************************

MPC ENCODER

*****************************************************************************/

LRESULT CALLBACK AUDIO_CONVERTER::Mpc(HWND hwnd, UINT msg,WPARAM wParam, LPARAM lParam)
{
        switch(msg)
        {
                       case WM_INITDIALOG:
                            {
                              General.CenterWindow(AudioConverter.AudioConverterHandle,hwnd);
                             
                              HWND hQuality=GetDlgItem(hwnd,IDC_ENC_MPC_QUALITY);
                             
                              SendMessage(hQuality,CB_ADDSTRING,0,(LPARAM)"Thumb(low/medium quality)");
                              SendMessage(hQuality,CB_ADDSTRING,1,(LPARAM)"Radio(medium quality)");
                              SendMessage(hQuality,CB_ADDSTRING,2,(LPARAM)"Standard(high quality)");
                              SendMessage(hQuality,CB_ADDSTRING,3,(LPARAM)"Extreme(excellent quality)");
                              SendMessage(hQuality,CB_ADDSTRING,4,(LPARAM)"Insane(for archiving)");
                             
                              AudioConverter.InitializeSettings_Mpc(hwnd);
                              
                              SetFocus(GetDlgItem(hwnd,IDC_ENC_MPC_OK));
                            }
                       break;
                       case WM_COMMAND:
                            switch(LOWORD(wParam))
                             {
                                case IDC_ENC_MPC_RESTORE:
                                    AudioConverter.mpc_default(hwnd);
                               break;
                               case IDC_ENC_MPC_CANCEL:
                                    EndDialog(hwnd,true);
                               break;
                               case IDC_ENC_MPC_OK:
                                    AudioConverter.StoreSettings_Mpc(hwnd);
                                    EndDialog(hwnd,true);
                               break;
                             }
                        break;        
                       case WM_CLOSE:
                            EndDialog(hwnd,true);
                       break;
        }
return false;
}


void AUDIO_CONVERTER::mpc_default(HWND hwnd)
{
     HWND hQuality=GetDlgItem(hwnd,IDC_ENC_MPC_QUALITY);
     
     SendMessage(hQuality,CB_SETCURSEL,2,0);
}


void AUDIO_CONVERTER::InitializeSettings_Mpc(HWND hwnd)
{
     int temp;
     HWND hQuality=GetDlgItem(hwnd,IDC_ENC_MPC_QUALITY);
     
     temp=Registry.ReadNumberSetting(MPC_ENCODER_PATH,"quality");
     
     if(temp==-1) temp=2;
     Registry.WriteNumberSetting(MPC_ENCODER_PATH,"quality",temp);
     SendMessage(hQuality,CB_SETCURSEL,temp,0);
     
     
}

void AUDIO_CONVERTER::StoreSettings_Mpc(HWND hwnd)
{
     HWND hQuality=GetDlgItem(hwnd,IDC_ENC_MPC_QUALITY);
     
     Registry.WriteNumberSetting(MPC_ENCODER_PATH,"quality",SendMessage(hQuality,CB_GETCURSEL,0,0));
    
}




/*****************************************************************************

OGG ENCODER

*****************************************************************************/

LRESULT CALLBACK AUDIO_CONVERTER::Ogg(HWND hwnd, UINT msg,WPARAM wParam, LPARAM lParam)
{
        switch(msg)
        {
         case WM_INITDIALOG:
              {
               General.CenterWindow(AudioConverter.AudioConverterHandle,hwnd);
               
               HWND hQuality=GetDlgItem(hwnd,IDC_ENC_OGG_QUALITY);
            //   HWND 
               
              }
         break;
         case WM_CLOSE:
           EndDialog(hwnd,true);
         break;
        }
return FALSE;
}


void AUDIO_CONVERTER::ogg_default(HWND hwnd)
{
    
}


void AUDIO_CONVERTER::InitializeSettings_ogg(HWND hwnd)
{
    
     
}

void AUDIO_CONVERTER::StoreSettings_ogg(HWND hwnd)
{
    
}


/*****************************************************************************

SPX ENCODER

*****************************************************************************/

LRESULT CALLBACK AUDIO_CONVERTER::Spx(HWND hwnd, UINT msg,WPARAM wParam, LPARAM lParam)
{
        switch(msg)
        {
         case WM_INITDIALOG:
           General.CenterWindow(AudioConverter.AudioConverterHandle,hwnd);   
         break;
         case WM_CLOSE:
           EndDialog(hwnd,true);
         break;
        }
return FALSE;
}


void AUDIO_CONVERTER::spx_default(HWND hwnd)
{
    
}


void AUDIO_CONVERTER::InitializeSettings_spx(HWND hwnd)
{
    
     
}

void AUDIO_CONVERTER::StoreSettings_spx(HWND hwnd)
{
    
}

LRESULT CALLBACK AUDIO_CONVERTER::About(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
        switch(msg)
         {
           case WM_INITDIALOG:
                {
                  General.CenterWindow(AudioConverter.AudioConverterHandle,hwnd);
                  HWND myphoto=CreateWindowExA(0,"static",NULL,WS_CHILD|WS_VISIBLE|SS_BITMAP,
                                30,10,0,0,hwnd,NULL,NULL,NULL);
                   SendMessage(myphoto,STM_SETIMAGE,IMAGE_BITMAP,
                             (LPARAM)(HANDLE)LoadBitmapA(GetModuleHandle(NULL),MAKEINTRESOURCE(1002)));
                   CreateWindowEx(0,"static","Name:ROSHAN BHATTA",WS_CHILD|WS_VISIBLE,28,290,160,15,hwnd,NULL,NULL,NULL);
                   CreateWindowEx(0,"static","Phone:+977-9841072258",WS_CHILD|WS_VISIBLE,28,310,160,15,hwnd,NULL,NULL,NULL);
                   CreateWindowEx(0,"static","Email:roshan_bhatta01@hotmail.com",WS_CHILD|WS_VISIBLE,28,330,260,16,hwnd,NULL,NULL,NULL);
                   SetFocus(GetDlgItem(hwnd,1001));
                }
           break;
           case WM_COMMAND:
            {
             switch(LOWORD(wParam))
              {
               case 1001:
                 {
                  EndDialog(hwnd,true);
                 }
                break; 
            }
           }
           break;
           case WM_CTLCOLORSTATIC:
                {
                 SetTextColor((HDC)wParam,RGB(255,0,0));
                 SetBkMode((HDC)wParam,TRANSPARENT);
                 return (LRESULT)CreateSolidBrush(RGB(0,0,0));
                }
           break;
           case WM_CTLCOLORDLG:
                {
                 return (LRESULT)CreateSolidBrush(RGB(0,0,0));
                }
           break;
           case WM_CLOSE:
                EndDialog(hwnd,true);
           break;
         }
  return FALSE;
}

LRESULT CALLBACK AUDIO_CONVERTER::Settings(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
        switch(msg)
         {
                   case WM_INITDIALOG:
                        {
                         General.CenterWindow(AudioConverter.AudioConverterHandle,hwnd);
                         
                         char *output_path=Registry.ReadStringSetting(AUDIO_CONVERTER_PATH,"output");
                         if(!stricmp(output_path,""))
                          SendMessage(GetDlgItem(hwnd,1003),WM_SETTEXT,0,(LPARAM)General.GetMyMusicDir());
                         else
                          SendMessage(GetDlgItem(hwnd,1003),WM_SETTEXT,0,(LPARAM)output_path);
                         
                         /*
                           cmsg--msg after conversion
                           dmsg--msg after error
                           wmsg--warning before shutdown/restart
                         */
                         
                        
                         char *msg=Registry.ReadStringSetting(AUDIO_CONVERTER_PATH,"cmsg");
                         if(!stricmp(msg,"1"))
                          SendMessage(GetDlgItem(hwnd,1005),BM_SETCHECK,BST_CHECKED,0);
                         
                         msg=Registry.ReadStringSetting(AUDIO_CONVERTER_PATH,"dmsg");
                          if(!stricmp(msg,"1"))
                           SendMessage(GetDlgItem(hwnd,1006),BM_SETCHECK,BST_CHECKED,0);
                         
                         msg=Registry.ReadStringSetting(AUDIO_CONVERTER_PATH,"wmsg");
                          if(!stricmp(msg,"1"))
                           SendMessage(GetDlgItem(hwnd,1007),BM_SETCHECK,BST_CHECKED,0);                           
                          
                         SetFocus(GetDlgItem(hwnd,1008));
                        }
                   break;
                   case WM_COMMAND:
                        switch(LOWORD(wParam))
                         {
                          case 1004://This is browse button
                           {
                             char *path=Shell.BrowseForFolder(hwnd);
                             if(stricmp(path,""))
                             {
                              SendMessage(GetDlgItem(hwnd,1003),WM_SETTEXT,0,(LPARAM)path);
                              Registry.WriteStringSetting(AUDIO_CONVERTER_PATH,"output",path);
                             }
                           }
                          break;
                          case 1005://this is cmsg
                           {
                               UINT msg=SendMessage(GetDlgItem(hwnd,1005),BM_GETCHECK,0,0);
                               
                               if(msg==BST_CHECKED)
                                {
                                 Registry.WriteStringSetting(AUDIO_CONVERTER_PATH,"cmsg","1");
                                }
                               else
                                {
                                 Registry.WriteStringSetting(AUDIO_CONVERTER_PATH,"cmsg","0");
                                } 
                           }
                          break;
                          case 1006://this is dmsg
                           {
                               UINT msg=SendMessage(GetDlgItem(hwnd,1006),BM_GETCHECK,0,0);
                               
                               if(msg==BST_CHECKED)
                                {
                                 Registry.WriteStringSetting(AUDIO_CONVERTER_PATH,"dmsg","1");
                                }
                               else
                                {
                                 Registry.WriteStringSetting(AUDIO_CONVERTER_PATH,"dmsg","0");
                                } 
                           }
                          break;
                          case 1007://this is wmsg
                           {
                               UINT msg=SendMessage(GetDlgItem(hwnd,1007),BM_GETCHECK,0,0);
                               
                               if(msg==BST_CHECKED)
                                {
                                 Registry.WriteStringSetting(AUDIO_CONVERTER_PATH,"wmsg","1");
                                }
                               else
                                {
                                 Registry.WriteStringSetting(AUDIO_CONVERTER_PATH,"wmsg","0");
                                } 
                           }
                          break;
                          case 1008://this is ok button
                           {
                             EndDialog(hwnd,true);
                           }
                          break;
                         }
                   break;
                   case WM_CLOSE:
                        EndDialog(hwnd,true);
                   break;
         }
 return FALSE;
}
