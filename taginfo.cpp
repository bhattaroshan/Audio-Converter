
#include"taginfo.h"
#include"globaldata.h"


const char* TAGINFO::GetSong_Title(DWORD nFile)
{
    return TAGS_Read(nFile,"%IFV2(%TITL,%ICAP(%TITL),No Title)");
};

const char* TAGINFO::GetSong_Album(DWORD nFile)
{
    return TAGS_Read(nFile,"%IFV2(%ALBM,%ICAP(%ALBM),No Album)");
};

const char* TAGINFO::GetSong_Artist(DWORD nFile)
{
    return TAGS_Read(nFile,"%IFV2(%ARTI,%ICAP(%ARTI),No Artist)");
};

const char* TAGINFO::GetSong_Year(DWORD nFile)
{
    return TAGS_Read(nFile,"%IFV2(%YEAR,%YEAR,--)");
};

const char* TAGINFO::GetSong_Genre(DWORD nFile)
{
    return TAGS_Read(nFile,"%IFV2(%GNRE,%GNRE,--)");
};

const char* TAGINFO::GetSong_Track(DWORD nFile)
{
    return TAGS_Read(nFile,"%IFV2(%TRCK,%TRCK,--)");
};

const char* TAGINFO::GetSong_Comment(DWORD nFile)
{
    return TAGS_Read(nFile,"%IFV2(%CMNT,%CMNT,--)");
};

const char* TAGINFO::GetSong_CopyWrite(DWORD nFile)
{
    return TAGS_Read(nFile,"%IFV2(%COPY,%COPY,--)");
};

const char *GetSong_Composer(DWORD nFile)
{
    return TAGS_Read(nFile,"%IFV2(%COMP,%COMP,--)");
};

