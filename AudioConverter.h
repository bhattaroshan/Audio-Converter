
#ifndef AUDIOCONVERTER_H
#define AUDIOCONVERTER_H


#include<windows.h>
#include<commctrl.h>
#include<stdio.h>
#include"bass.h"
#include"bassenc.h"

class AUDIO_CONVERTER
{
      public:
             AUDIO_CONVERTER();
             HWND AudioConverterDialog();
             static LRESULT CALLBACK AudioConverterProc(HWND,UINT,WPARAM,LPARAM);
             void AddMenu(HWND);
             
             void CreateButtonControls(HWND);
             void CreateProgressBar(HWND);
             void CreateComboBox(HWND);
             void CreateToolbarControls(HWND);
             void AddToolbarButton(HWND,TBBUTTON&,int=6,int=-1,LPSTR=NULL,int=-1);
             
             
             void CreateStatusbarControl(HWND);
             
             
             void CreateListViewControls(HWND,int,int,int,int);
             void AddListViewColumn(HWND,int,int,LPSTR,UINT=LVCF_WIDTH|LVCF_TEXT|LVCF_FMT,int=LVCFMT_LEFT); 
             void AddListViewItem(HWND,int,LPSTR,...);
             char *GetListViewColumnText(HWND,int,int);
             char *GetPathFromListView(HWND,int);
             void ListViewChangeImage(HWND,int,int);
             void FindFile_Recursive(char *,char *);
             void SetPathToList(char *szBuffer);
             void ListView_SetItemSelect(HWND hwnd,int nIndex,bool bSelected,bool bFocused);
             
             void OnTimer(HWND);
             void OnSize(HWND,WPARAM,LPARAM);
             void OnNotify(HWND,LPARAM);
             void OnDropFile(HWND,WPARAM);
             void OnRemoveAll();
             void OnRemove();
             void OnExplore();
             void OnProperties();
             void FolderExplore();
             void AddFiles();
             void AddFolder();
             void OnConvert();
             void OnPause();
             void OnStop();
             
             
             void GetTotalTime(int&,char*);
             void GetCurrentTotalTime(HSTREAM,int&,char*);
             void GetCurrentTime(HSTREAM,int&,char*);
             void ShowTime(HWND,char*,char*);
             char*MakeTime(int);
            
             void LoadPlugin();
             void SplitTime(int,int&,int&,int&);
             
             void GetEncoderDialog(HWND);
             
             //DIALOGS.............................
             static LRESULT CALLBACK AudioFilter(HWND,UINT,WPARAM,LPARAM);
             static LRESULT CALLBACK        Flac(HWND,UINT,WPARAM,LPARAM);
             static LRESULT CALLBACK         Mpc(HWND,UINT,WPARAM,LPARAM);
             static LRESULT CALLBACK         Aac(HWND,UINT,WPARAM,LPARAM);
             static LRESULT CALLBACK         Mp3(HWND,UINT,WPARAM,LPARAM);
             static LRESULT CALLBACK         Ogg(HWND,UINT,WPARAM,LPARAM);
             static LRESULT CALLBACK         Spx(HWND,UINT,WPARAM,LPARAM);
             static LRESULT CALLBACK  Properties(HWND,UINT,WPARAM,LPARAM);
             static LRESULT CALLBACK       About(HWND,UINT,WPARAM,LPARAM);
             static LRESULT CALLBACK    Settings(HWND,UINT,WPARAM,LPARAM);
             //ENCODER Defaults...................
             void GetConverterReference();
             
             //FLAC FUNCTIONS
             void flac_default(HWND);
             void InitializeSettings_Flac(HWND);
             void StoreSettings_Flac(HWND);
             
             //MPC FUNCTIONS
             void mpc_default(HWND);
             void InitializeSettings_Mpc(HWND);
             void StoreSettings_Mpc(HWND);
             
             //AAC FUNCTIONS
             void aac_default(HWND);
             void InitializeSettings_aac(HWND);
             void StoreSettings_aac(HWND);
             
             //MP3 FUNCTIONS
             void mp3_default(HWND);
             void InitializeSettings_mp3(HWND);
             void StoreSettings_mp3(HWND);
             
             //OGG FUNCTIONS
             void ogg_default(HWND);
             void InitializeSettings_ogg(HWND);
             void StoreSettings_ogg(HWND);
             
             //SPX FUNCTIONS
             void spx_default(HWND);
             void InitializeSettings_spx(HWND);
             void StoreSettings_spx(HWND);
             
             void InitializeSettings();
             void StoreSettings();
             
             
             
             void ExitConverter(HWND);
             
      private:
              HIMAGELIST    ToolbarImageList;
              HIMAGELIST    ListViewImageList;
              HIMAGELIST    ConvertImageList;
              HIMAGELIST    PauseImageList;
              HIMAGELIST    StopImageList;
              
              HWND          AudioConverterHandle;
              HWND          ToolbarHandle;
              HWND          ListViewHandle;
              HWND          StatusbarHandle;
              
              HWND          ConvertButtonHandle;
              HWND          PauseButtonHandle;
              HWND          StopButtonHandle;
              HWND          EncoderButtonHandle;
              HWND          ProgressHandle;
              HWND          OverallProgressHandle;
              HWND          ComboBoxHandle;
              HWND          StaticHandle1;
              HWND          StaticHandle2;
              HWND          StaticHandle3;
              HWND          TimeHandle1;
              HWND          TimeHandle2;
              HWND          ProgressGroupHandle;
              HWND          FormatGroupHandle;
              
              HFONT         StaticFont1;
              HFONT         StaticFont2;
              HFONT         StaticFont3;
              
              char          szEditText[21];
              char          szStatus[MAX_PATH+1];
              
              bool          timer_on;
              
             //BASS.................................
              int           TotalItem;
              int           CurrentItem;
              HSTREAM       nChannel;
              HENCODE       nEncode;
              //Initialization
              bool         IsMaximize;
              bool         convert_exception;
              bool         button_exception;
              
              //TIME VARIABLES
              int          TotalTime;
              int          TotalPositionInitial;
              int          TotalPositionFinal;
              int          CurrentTime;
              int          CurrentTotalTime;
              int          Progress1;
              int          Progress2;
              
              char         szTotalTime[30];
              char         szCurrentTime[30];
              char         szCurrentTotalTime[30];
              char         szTotalPosition[30];
              char         szCurrentFileName[MAX_PATH];
              char         szActualFile[MAX_PATH];
              
              //CONVERTER VARIABLE
              char         Conv_Ref[MAX_PATH+100];
              
              
              //MENU VARIABLE
              bool         list_click;
              bool         on_convert_ontop;
              HMENU        MenuHandle;
              
              //FILE INFORMATION
              HWND         FileInfo_Tab;
              HWND         FileInfo_OK;
              HWND         FileInfo_Static1;
              HWND         FileInfo_Static2;
              HWND         FileInfo_Static3;
              HWND         FileInfo_Static4;
              HWND         FileInfo_Edit1;
              HWND         FileInfo_Edit2;
              HWND         FileInfo_Edit3;
              
};


#endif
