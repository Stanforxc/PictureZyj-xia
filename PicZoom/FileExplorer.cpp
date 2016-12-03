#include "StdAfx.h"
#include "FileExplorer.h"

FileExplorer::FileExplorer(void)
{
    m_hSearchComplete = ::CreateEvent( 0, true, false, 0 );
    m_bRepeatOn = false;
}

FileExplorer::~FileExplorer(void)
{
//    m_pFileFinder.Close();
    CloseHandle( m_hSearchComplete );
}
void FileExplorer::SetFilePath( const TCHAR* pNewFileName_i )
{
    m_nCurrentIndex = 0;
    m_csCurrentFileName = CString(pNewFileName_i);
    m_csPreviousFileName = m_csCurrentFileName;
    // Here Find folder name.
    CString csFileName( pNewFileName_i );
    int nEndOfFolderName = csFileName.ReverseFind( '\\' );
    csFileName.Truncate( nEndOfFolderName + 1 );
    csFileName += L"\*.*";
    m_csFolderName = csFileName;

    // Reset search start event, After search complete, this event is 
    // set by the thread.
    ResetEvent( m_hSearchComplete );
    AfxBeginThread( FindImageFileNames, this );
}

UINT FileExplorer::FindImageFileNames( LPVOID pParam_i )
{
    FileExplorer* pThis = (FileExplorer*) pParam_i;
    pThis->FindImageFileNameHandler();
    return 1;
}
void FileExplorer::FindImageFileNameHandler()
{
    ImageFileFind FileFinder;
    m_ImageFiles.clear();
    m_nCurrentIndex = 0;

    // Find Image files and put it in Vector.
    if( FileFinder.FindFile( m_csFolderName, 0 ))
    {
        // Seek to Next File
        while( FileFinder.FindNextFile())
        {
            CString csCurrentFile = FileFinder.GetFilePath();
            if( 0 == m_csCurrentFileName.Compare( csCurrentFile ))
            {
                // Now found current File.
                m_nCurrentIndex = m_ImageFiles.size();
            }

            // Put image file name to the vector.
            m_ImageFiles.push_back( csCurrentFile );
        }
    }
    FileFinder.Close();

    int nErr = GetLastError();
    // Now all image file names are pushed to the list, and the 
    // Event is signaled.
    SetEvent( m_hSearchComplete );
}

CString FileExplorer::GetNextFileName()
{
    WaitForSingleObject( m_hSearchComplete, 0 );

    if( m_nCurrentIndex < 0 || m_nCurrentIndex >= m_ImageFiles.size() )
    {
        return L"";
    }

    if( m_bRepeatOn )
    {
        m_nCurrentIndex++;
        if( m_nCurrentIndex >= m_ImageFiles.size())
        {
            // Move to first image.
            m_nCurrentIndex = 0;
        }
    }
    else
    {
        if( m_nCurrentIndex < ( m_ImageFiles.size() - 1))
        {
            // Now increment the current file name index and take that from vector.
            m_nCurrentIndex++;
        }
    }

    return m_ImageFiles[m_nCurrentIndex];
}
CString FileExplorer::GetPreviousFileName()
{
    WaitForSingleObject( m_hSearchComplete, 0 );

    if( m_nCurrentIndex < 0 || m_nCurrentIndex >= m_ImageFiles.size() )
    {
        return L"";
    }

    if( m_bRepeatOn )
    {

        m_nCurrentIndex--;
        if( m_nCurrentIndex < 0 )
        {
            m_nCurrentIndex = m_ImageFiles.size() - 1;
        }
    }
    else
    {

        if( m_nCurrentIndex > 0)
        {
            // Now decrement the current file name index and take that from vector.
            m_nCurrentIndex--;
        }
    }
    return m_ImageFiles[m_nCurrentIndex];
}

CString FileExplorer::GetCurrentName()
{
    return m_ImageFiles[m_nCurrentIndex];
}

void FileExplorer::SetRepeatState( const bool bRepeatOn_i )
{
    m_bRepeatOn = bRepeatOn_i;
}