

#include"registry.h"

int REGISTRY::ReadNumber(HKEY hKey,LPSTR nSubKey,LPSTR nKeyValue)
{
    HKEY hRegKey;
    DWORD nBuffer=MAX_PATH;
    char szValue[MAX_PATH]="\0";
    LONG value;
    
    RegOpenKeyExA(HKEY_CURRENT_USER,nSubKey,0,KEY_QUERY_VALUE,&hRegKey);
    value=RegQueryValueEx(hRegKey,nKeyValue,NULL,NULL,(LPBYTE)szValue,&nBuffer);
    RegCloseKey(hRegKey);
    if(value!=ERROR_SUCCESS) return -1;
    
    return(atoi(szValue));
}


char* REGISTRY::ReadString(HKEY hKey,LPSTR nSubKey,LPSTR nKeyValue)
{
    HKEY hRegKey;
    DWORD nBuffer=MAX_PATH;
    static char szValue[MAX_PATH]="\0";
    LONG value;
    
    RegOpenKeyExA(HKEY_CURRENT_USER,nSubKey,0,KEY_QUERY_VALUE,&hRegKey);
    value=RegQueryValueEx(hRegKey,nKeyValue,NULL,NULL,(LPBYTE)szValue,&nBuffer);
    RegCloseKey(hRegKey);
    if(value!=ERROR_SUCCESS) return "";
    
    return szValue;
}

void REGISTRY::WriteNumber(HKEY hKey,LPSTR nSubKey,LPSTR nClass,int nKeyValue)
{
     HKEY regkey;
     DWORD disposition;
     char szBuffer[255];
     itoa(nKeyValue,szBuffer,10);
     RegCreateKeyEx(hKey,nSubKey,0,"",REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&regkey,&disposition);
     RegSetValueEx(regkey,nClass,0,REG_SZ,(CONST BYTE *)szBuffer,strlen(szBuffer)+1);
     RegCloseKey(regkey);
}

void REGISTRY::WriteString(HKEY hKey,LPSTR nSubKey,LPSTR nClass,LPSTR nKeyValue)
{
     HKEY regkey;
     DWORD disposition;
     
     RegCreateKeyEx(hKey,nSubKey,0,"",REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&regkey,&disposition);
     RegSetValueEx(regkey,nClass,0,REG_SZ,(CONST BYTE *)nKeyValue,strlen(nKeyValue)+1);
     RegCloseKey(regkey);
}

void REGISTRY::WriteNumberSetting(LPSTR nSubKey,LPSTR nClass,int nKeyValue)
{
     WriteNumber(HKEY_CURRENT_USER,nSubKey,nClass,nKeyValue);
}

void REGISTRY::WriteStringSetting(LPSTR nSubKey,LPSTR nClass,LPSTR nKeyValue)
{
     WriteString(HKEY_CURRENT_USER,nSubKey,nClass,nKeyValue);
}

int REGISTRY::ReadNumberSetting(LPSTR nSubKey,LPSTR nClass)
{
    return ReadNumber(HKEY_CURRENT_USER,nSubKey,nClass);
}

char* REGISTRY::ReadStringSetting(LPSTR nSubKey,LPSTR nClass)
{
    return ReadString(HKEY_CURRENT_USER,nSubKey,nClass);
}

