// PicZoom.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CPicZoomApp:
// Application class of PicZoom.
// When a new instance of PicaZoom starts, it checks already PicZoom running.
// If one instance is running, then send a message to the old one, and exit.
//

class CPicZoomApp : public CWinApp
{
public:
	CPicZoomApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	BOOL RunADialog();

	DECLARE_MESSAGE_MAP()
};

extern CPicZoomApp theApp;