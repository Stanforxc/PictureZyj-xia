#include "StdAfx.h"
#include "NewFileLoader.h"
#include "Consts.h"

NewFileLoader::NewFileLoader(HWND hParentWindow)
{
    m_bExit = false;
    m_hParentWindow = hParentWindow;
    AfxBeginThread( ObserveThread, this );

}

NewFileLoader::~NewFileLoader(void)
{
    m_bExit = true;
    HANDLE hNextFileOpen = ::OpenEvent( EVENT_ALL_ACCESS, true, L"PicZoomNewFile" );
    ::SetEvent( hNextFileOpen );
}

UINT NewFileLoader::ObserveThread(LPVOID pParam)
{
    NewFileLoader* pInstance = (NewFileLoader*) pParam;
    HANDLE hNextFileOpen = ::OpenEvent( EVENT_ALL_ACCESS, true, L"PicZoomNewFile" );

    while( 1 )
    {
        WaitForSingleObject( hNextFileOpen, INFINITE );

        // Ensure event set from other process.
        if( pInstance->m_bExit )
        {
            return 1;
        }

        HANDLE hSharedFile = ::OpenFileMapping(PAGE_READONLY, true, L"PicZoomNewFileName" );
        if( NULL == hSharedFile)
        {
            AfxMessageBox( L"File open failed" );
        }
        // Read data from shared memory.
        LPTSTR ptFileName = (LPTSTR)::MapViewOfFile( hSharedFile, FILE_MAP_ALL_ACCESS, 0, 0, 1024 );
        
        // File name allocated in new buffer, and this buffer will be de allocated by message handler.
        TCHAR* pNewBuffer = new TCHAR[wcslen( ptFileName ) + 1];
        memset( pNewBuffer, 0, (wcslen( ptFileName ) + 1) * 2 );
        wcscpy( pNewBuffer, ptFileName );
        // Validate FileName with required extension.
        CString csCurrentFile = CString( pNewBuffer );
        csCurrentFile = csCurrentFile.Right( csCurrentFile.GetLength() - 1 );
        csCurrentFile = csCurrentFile.Left( csCurrentFile.GetLength() - 1 );
        CString csExtension = csCurrentFile.Right( 4 );
        int nRet = csExtension.CompareNoCase( L".JPG" );;
        if(( 0 == csExtension.CompareNoCase( L".bmp" ) ||
            0 == csExtension.CompareNoCase( L".JPG" ) ||
            0 == csExtension.CompareNoCase( L".png" ) ||
            0 == csExtension.CompareNoCase( L".gif" ) ||
            0 == csExtension.CompareNoCase(L".tga" ) ))
        {
            PostMessage( pInstance->m_hParentWindow, WM_NEW_FILE_LOAD, 0, (LPARAM)pNewBuffer );
        }
    }
    return 1;
}
