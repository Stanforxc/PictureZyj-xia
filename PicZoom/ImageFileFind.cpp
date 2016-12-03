#include "StdAfx.h"
#include "ImageFileFind.h"

ImageFileFind::ImageFileFind(void)
{
    m_bReachedEnd = false;
}

ImageFileFind::~ImageFileFind(void)
{
}

// This function return true if image file found, else return false.
BOOL ImageFileFind::FindNextFile()
{
    if( m_bReachedEnd )
    {
        return false;
    }
    // Seek to First image File
    while( CFileFind::FindNextFile())
    {
        CString csCurrentFile = CFileFind::GetFilePath();
        CString csExtension = csCurrentFile.Right( 4 );
        int nRet = csExtension.CompareNoCase( L".JPG" );;
        if(( 0 == csExtension.CompareNoCase( L".bmp" ) ||
            0 == csExtension.CompareNoCase( L".JPG" ) ||
            0 == csExtension.CompareNoCase( L".png" ) ||
            0 == csExtension.CompareNoCase( L".gif" ) ||
            0 == csExtension.CompareNoCase(L".tga" ) ))
        {
            return true;
        }
    }
    // Check last file is Image or Not
    CString csCurrentFile = CFileFind::GetFilePath();
    CString csExtension = csCurrentFile.Right( 4 );
    int nRet = csExtension.CompareNoCase( L".JPG" );;
    if(( 0 == csExtension.CompareNoCase( L".bmp" ) ||
        0 == csExtension.CompareNoCase( L".JPG" ) ||
        0 == csExtension.CompareNoCase( L".png" ) ||
        0 == csExtension.CompareNoCase( L".gif" ) ||
        0 == csExtension.CompareNoCase(L".tga" ) ))
    {
        m_bReachedEnd = true;
        return true;
    }
    m_bReachedEnd = true;
    return false;
}
