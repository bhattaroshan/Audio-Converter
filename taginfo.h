
#ifndef TAGINFO_H
#define TAGINFO_H

#include<windows.h>

class TAGINFO
{
      public:
             const char *GetSong_Title(DWORD);
             const char *GetSong_Album(DWORD);
             const char *GetSong_Artist(DWORD);
             const char *GetSong_Year(DWORD);
             const char *GetSong_Genre(DWORD);
             const char *GetSong_Track(DWORD);
             const char *GetSong_Comment(DWORD);
             const char *GetSong_CopyWrite(DWORD);
             const char *GetSong_Composer(DWORD);
};

#endif
