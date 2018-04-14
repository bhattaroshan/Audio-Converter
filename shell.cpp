
#include"shell.h"


int SHELL::SHProperties(LPSTR szPath)
{
    SHELLEXECUTEINFO shi;
	memset(&shi, 0, sizeof(SHELLEXECUTEINFO));
	shi.cbSize 		 = sizeof(SHELLEXECUTEINFO);
	shi.fMask 		 = SEE_MASK_INVOKEIDLIST ;
	shi.hwnd 		 = NULL;
	shi.lpVerb 		 = "properties";
	shi.lpFile 		 = szPath;
	shi.lpParameters = "";
	shi.lpDirectory  = NULL;
	shi.nShow 		 = SW_SHOW;
	shi.hInstApp 	 = NULL;
	return ShellExecuteEx(&shi);
}

char* SHELL::BrowseForFolder(HWND hwnd)
{
      LPITEMIDLIST item;
      static char  szFolderPath[MAX_PATH+1]="";
      BROWSEINFO   bi;
      
      ZeroMemory(&bi,sizeof bi);
      
      bi.hwndOwner=hwnd;
      bi.lpszTitle="Browse For Folder";
      
      item=SHBrowseForFolder(&bi);
      SHGetPathFromIDList(item,szFolderPath);
      
      return szFolderPath;
}

char* SHELL::FileSelectDialog(HWND hwnd,LPSTR szFilter)
{
     static char szFileName[MAX_PATH+1]="\0";
     OPENFILENAME ofn;
     
     ZeroMemory(&ofn,sizeof ofn);
      szFileName[0] = 0;
     ofn.lStructSize=sizeof ofn;
     ofn.hwndOwner=hwnd;
     ofn.lpstrTitle="Choose a file";
     ofn.lpstrFilter=szFilter;
     ofn.lpstrFile=szFileName;
     ofn.nMaxFile=sizeof szFileName + 1;
     GetOpenFileName(&ofn);
 
     if(strlen(szFileName)==0) return "";
     
     return szFileName;
}
