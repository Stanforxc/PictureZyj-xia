// PicZoom.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "PicZoom.h"
#include "PicZoomDlg.h"
#include "afxmt.h"
#include<iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPicZoomApp

BEGIN_MESSAGE_MAP(CPicZoomApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()



// CPicZoomApp construction

CPicZoomApp::CPicZoomApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CPicZoomApp object
//
CPicZoomApp theApp;


// CPicZoomApp initialization

BOOL CPicZoomApp::InitInstance()
{
    CMutex Mutex( false, L"PicZoomApplication", 0 );
    if( WAIT_OBJECT_0 != WaitForSingleObject( Mutex.m_hObject, 0 ))
    {
        // Here another instance of PicZoom is running, now send Message to 
        // PicZoom to load the current file.
        // Read data from shared memory.
        HANDLE hSharedFile = ::OpenFileMapping(PAGE_READONLY, true, L"PicZoomNewFileName" );
        LPTSTR ptFileName = (LPTSTR)::MapViewOfFile( hSharedFile, FILE_MAP_ALL_ACCESS, 0, 0, 1024 );
        wcscpy( ptFileName, m_lpCmdLine );
        ::UnmapViewOfFile( ptFileName );

        HANDLE hNextFileOpen = ::OpenEvent( EVENT_ALL_ACCESS, true, L"PicZoomNewFile" );
        SetEvent( hNextFileOpen );
        return false;
    }
    else
    {
        HANDLE hNextFileOpen = ::CreateEvent( 0, false, false, L"PicZoomNewFile" );
        HANDLE hSharedFile = ::CreateFileMapping( INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0, 1024, L"PicZoomNewFileName" );
    }
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

    // Initialize OLE 2.0 libraries
    if (!AfxOleInit())
    {
        AfxMessageBox(L"AfxOleInit Error!");
        return FALSE;
    }

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	

	AllocConsole();
	//TODO: relocate the instream and outstream
	freopen("CONOUT$","w+t",stdout);
	freopen("CONIN$","r+t",stdin);
	
	
	


	/*
	system("pause");
	CPicZoomDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}*/

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
	
}

