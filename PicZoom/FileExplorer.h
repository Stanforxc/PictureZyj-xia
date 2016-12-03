#include "ImageFileFind.h"
#include <vector>
#pragma once

/*
This class is responsible for handling the next file and previous file
providing to the Dialog class. Whenever user press Next,Previous, Dialog class
calls GetNextFileName to retrieve the name of Next/Previous file.
On changing the Folder, SetFilePath of this class is called by Dialog.

For performance reasons, this class create a vector and hold all image files in that 
vector. This vector is created by a new thread. 
*/
class FileExplorer
{
public:
    FileExplorer(void);
    ~FileExplorer(void);

    void SetFilePath( const TCHAR* pNewFileName_i );
    CString GetNextFileName();
    CString GetPreviousFileName();
    CString GetCurrentName();
    void SetRepeatState( const bool bRepeatOn_i );
    static UINT FindImageFileNames( LPVOID pParam_i );
    void FindImageFileNameHandler();

private:

    //ImageFileFind m_pFileFinder;
    CString m_csFolderName;
    CString m_csCurrentFileName;
    CString m_csPreviousFileName;
    std::vector<CString> m_ImageFiles;
    int     m_nCurrentIndex;
    HANDLE  m_hSearchComplete;
    bool    m_bRepeatOn;
};
