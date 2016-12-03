// PicZoomDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PicZoom.h"
#include "PicZoomDlg.h"
#include "GLSetup.h"
#include "BMPLoader.h"
#include "GLTexture.h"
#include "GLVertexBuffer.h"
#include "ScreenCapture.h"
#include "math.h"
#include "atlconv.h"
#include "comdef.h"
#include "gdiplus.h"
#include "Consts.h"
#include "CloseButton.h"
#include "FontEngine.h"
#include "ZoomText.h"
#include "GLButton.h"
#include "CoordConverter.h"
#include "BottomWindows.h"
#include "FileExplorer.h"
#include "SlideShow.h"
#include "NewFileLoader.h"
#include "BackGround.h"
#include "ImageArea.h"
#include "CursorManager.h"
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAKE_CENTER 0.5f
#define ZOOM_TIMER_FACTOR 20.0f

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CPicZoomDlg dialog




CPicZoomDlg::CPicZoomDlg(TCHAR pFilePath[], CWnd* pParent /*=NULL*/ )
	: CDialog(CPicZoomDlg::IDD, pParent)
{
    m_bInitComplete = false;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_pGLSetup = 0;
    m_pSlideShow = 0;
    m_pNewFileLoader = 0;
    m_bStartupLogoDisplay = false;
	m_csFileURL = pFilePath;
}

void CPicZoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPicZoomDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
    ON_WM_CLOSE()
    ON_WM_MOUSEWHEEL()
    ON_WM_LBUTTONDOWN()
    ON_WM_RBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_TIMER()
    ON_WM_SYSCOMMAND()
    ON_WM_SETCURSOR()
    ON_WM_SIZE()
    ON_MESSAGE( WM_DROPFILES, OnDropFiles )
	ON_WM_QUERYDRAGICON()
    ON_MESSAGE( WM_ZOOM_PLUS_MESSAGE, OnZoomPlus )
    ON_MESSAGE( WM_ZOOM_MIN_MESSAGE, OnZoomMinus )
    ON_MESSAGE( WM_ZOOM_1TO1_MESSAGE, OnZoomOneToOne )
    ON_MESSAGE( WM_ROTATE_CLOCK_WISE, OnRotateClockWise )
    ON_MESSAGE( WM_ROTATE_ANTI_CLOCK_WISE, OnRotateAntiClockWise )
    ON_MESSAGE( WM_NEXT_FILE_OPEN, OnNextFileOpen )
    ON_MESSAGE( WM_PREVIOUS_FILE_OPEN, OnPreviousFileOpen )
    ON_MESSAGE( WM_PLAY_SLIDE_SHOW, OnSlideShow )
    ON_MESSAGE( WM_MAKE_TO_CENTER, OnMakeToCenter )
    ON_MESSAGE( WM_NEW_FILE_LOAD, OnNewFileLoad )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

ULONG_PTR m_gdiplusToken;

// CPicZoomDlg message handlers

BOOL CPicZoomDlg::OnInitDialog()
{
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

    m_pGLSetup = new GLSetup();
    // Creating cursor manager.
    CursorManager::CreateInstance( m_hWnd );

    m_nRotateOffSet = 0;
    m_bDisplayBottomWindow = true;
    m_pFileExplorer = new FileExplorer;
    m_nRotate = 0;
    m_eDisplayMode = BACKGROUND_MODE;

	CDialog::OnInitDialog();

    // Take Screen shot of Desktop.
    if( !m_ScreenCapture.TakeScreenShot())
    {
        AfxMessageBox( L"Screen shot failed" );
        return FALSE;
    }

    //m_csFileURL = CString( AfxGetApp()->m_lpCmdLine);
	/*
    if( m_csFileURL.GetLength() == 0 )
    {
        //m_bStartupLogoDisplay = true;
    }
    else
    {
        // Remove double quotes 
        m_csFileURL = m_csFileURL.Left( m_csFileURL.GetLength() - 1);
        m_csFileURL = m_csFileURL.Right( m_csFileURL.GetLength() - 1);
    }
	*/
	m_bStartupLogoDisplay = true;
    // Get Desktop Window region
    ::GetWindowRect( ::GetDesktopWindow(), &m_Desktop );
    m_Desktop.bottom = ::GetSystemMetrics( SM_CYFULLSCREEN ) + ::GetSystemMetrics( SM_CYSIZE );
    m_nDisplayWidth = m_Desktop.right;
    m_nDisplayHeight = m_Desktop.bottom;

    // Change Window size to Desktop size.
    SetWindowPos( 0, 0, 0, m_Desktop.right, m_Desktop.bottom, 0 );

    int nScreenWidth = 0;
    int nScrenHeight = 0;

    // Initialize Drawing Context.
    if( !m_pGLSetup->InitGL( m_hWnd ))
    {
        AfxMessageBox( L"OpenGL Initialisation failed" );
    }
    {
        int nError = glGetError();
        if( GL_NO_ERROR != nError )
        {
            AfxMessageBox( L"Opengl initilisation failed" );
            return false;
        }
    }
    // Ensure Width and height of texture is ok for dipsalying background image.
    int nMaxSize;
    glGetIntegerv( GL_MAX_TEXTURE_SIZE, (GLint*)&nMaxSize );

    if( nMaxSize < m_Desktop.right || nMaxSize < m_Desktop.bottom )
    {
        CString csError;
        csError.Format( L"Opengl supports Maximum Texture size is:%d.\n PicZoom requires %d to display background image ",
                         nMaxSize,max(m_Desktop.right, m_Desktop.bottom));
        return false;
    }

    //  Create Font for Drawing texts.
    m_pFontEngine = new FontEngine();
    {
        HDC hDc = ::GetDC( m_hWnd );
        if( !m_pFontEngine->Create( hDc ))
        {
            AfxMessageBox( L"Font Creation failed" );
        }
        ::ReleaseDC( m_hWnd, hDc );
    }


    // Create Close button.
    CloseButton* pCloseWnd = new CloseButton( m_hWnd );
    BottomWindows* pBottomWindows = new BottomWindows( m_hWnd, m_pFontEngine );
    // Add close button at first to the window list.
    m_Windows.push_back( pCloseWnd );
    // Add bottom windows to the window list.
    m_Windows.push_back( pBottomWindows );


    LoadImage( m_csFileURL );


    m_pFileExplorer->SetFilePath( m_csFileURL );

    // Initialisation after loading images, and proper parametetrs
    pCloseWnd->SetupButton();
    pCloseWnd->SetVisibility( true );
    pBottomWindows->SetVisibility( true );
    pBottomWindows->Initialize();

    pBottomWindows->SetMessage( L"Welcome To PicZoom Photo Viewer" );
    

	// IDM_ABOUTBOX must be in the  system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    glGetIntegerv( GL_VIEWPORT, viewport );

    SetWindowText( L"PicZoom Photo Viewer" );
    DragAcceptFiles(true);

    m_pNewFileLoader = new NewFileLoader( m_hWnd );
    SetTimer( TIMER_ZOOM, 10, 0 );
    m_bInitComplete = true;
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPicZoomDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
        static DISPLAY_MODE_e eOldState = m_eDisplayMode;
        int nSysCmd = nID & 0xFFF0;
        CDialog::OnSysCommand(nID, lParam);

        if( nSysCmd == SC_RESTORE || nSysCmd == SC_MAXIMIZE || nSysCmd == SC_ZOOM ||
            nSysCmd == WM_SIZE )
        {
            if( DIALOG_MODE == m_eDisplayMode )
            {
                SwitchModeToBackground();
            }
            else if( BACKGROUND_MODE == m_eDisplayMode )
            {
                SwitchModeToDialog();
            }
            if( SC_MAXIMIZE == nSysCmd || SC_RESTORE == nSysCmd )
            {
                // Check Minimize to Maximize
                if( MINIMIZE_MODE == m_eDisplayMode )
                {
                    if( DIALOG_MODE == eOldState )
                    {
                        m_eDisplayMode = DIALOG_MODE;
                        SwitchModeToDialog();
                    }
                }
            }
        }
        else if( SC_MINIMIZE == nSysCmd )
        {
            eOldState = m_eDisplayMode;
            m_eDisplayMode = MINIMIZE_MODE;
        }
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPicZoomDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
        CPaintDC dc(this);
        //CDialog::OnPaint();
        {
            Draw();
        }
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPicZoomDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPicZoomDlg::OnClose()
{
    // Stops all timers.
    for (int nTimer = TIMER_START + 1; nTimer < TIMER_MAX; nTimer++)
    {
        KillTimer( nTimer );
    }
    for( int nIndex = 0; nIndex < m_Windows.size(); nIndex++ )
    {
        GLWindowBase* pWindow = m_Windows[nIndex];
        delete pWindow;
    }
    m_Windows.clear();
    if( m_pFileExplorer )
    {
        delete m_pFileExplorer;
        m_pFileExplorer = 0;
    }
    if( m_pFontEngine )
    {
        delete m_pFontEngine;
        m_pFontEngine = 0;
    }
    
    if( m_pGLSetup )
    {
        delete m_pGLSetup;
        m_pGLSetup = 0;
    }
    if( m_pSlideShow )
    {
        delete m_pSlideShow;
        m_pSlideShow = 0;
    }

    CursorManager::DeleteInstance();

    Gdiplus::GdiplusShutdown( m_gdiplusToken );
    if( m_pGLSetup )
    {
        delete m_pGLSetup;
        m_pGLSetup = 0;
    }
    if( m_pNewFileLoader )
    {
        delete m_pNewFileLoader;
        m_pNewFileLoader = 0;
    }

    CDialog::OnClose();
}

BOOL CPicZoomDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // Send Mousewheel message to all windows.
    for( int nIndex = 0; nIndex < m_Windows.size(); ++nIndex )
    {
        m_Windows[nIndex]->OnMouseWheel( pt.x, pt.y, nFlags, zDelta );
    }

    // Hide Bottom windows

    BottomWindows* pBottomWindows = ( BottomWindows* )GetFromWindowList( L"BottomWindows" );
    pBottomWindows->SetVisibility( false );
    return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CPicZoomDlg::OnTimer( UINT_PTR nIDEvent )
{
    if( TIMER_START < nIDEvent && TIMER_MAX > nIDEvent )
    {
        Draw();
    }
}

void CPicZoomDlg::Draw()
{
    if( SLIDE_SHOW_MODE == m_eDisplayMode )
    {
        m_pSlideShow->Display();
    }
    else if( BACKGROUND_MODE == m_eDisplayMode ||
             DIALOG_MODE == m_eDisplayMode )
    {
        glClearColor( 0.5, 0.5, 0.5, 1.0 );
        // Drawing with gl setup.
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        for( int nWindowIndex = m_Windows.size() - 1; nWindowIndex >= 0 ; --nWindowIndex )
        {
            m_Windows[nWindowIndex]->Draw();
        }
    }

    m_pGLSetup->Draw();
}

void CPicZoomDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    // Avoid mouse messages in Slideshow and Unknown mode.
    if( BACKGROUND_MODE == m_eDisplayMode ||
        DIALOG_MODE == m_eDisplayMode )
    {
        POINT ptClient = point;
        for( int nWindowIndex = 0; nWindowIndex < m_Windows.size(); ++nWindowIndex )
        {
            if( m_Windows[nWindowIndex]->OnMouseMove( point.x, point.y, nFlags ))
            {
                return;
            }
        }

        if( MK_LBUTTON == ( nFlags & MK_LBUTTON ) )
        {
            if( DIALOG_MODE == m_eDisplayMode && m_bDlgMoveEnabled )
            {
                // Handle Dialog move 
                int nXDiff = point.x - m_ptDlgMoveStart.x;
                int nYDiff = point.y - m_ptDlgMoveStart.y;
                m_DlgRegion.left += nXDiff;
                m_DlgRegion.right += nXDiff;
                m_DlgRegion.top += nYDiff;
                m_DlgRegion.bottom += nYDiff;

                int nTitleBarHeight = ::GetSystemMetrics( SM_CYSIZE );

                SetWindowPos( 0,m_DlgRegion.left, m_DlgRegion.top, m_DlgRegion.right - m_DlgRegion.left,m_DlgRegion.bottom - m_DlgRegion.top + nTitleBarHeight, SWP_SHOWWINDOW );
                SetCapture();
            }
        }
    }
    CDialog::OnMouseMove(nFlags, point);
}

void CPicZoomDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    if( SLIDE_SHOW_MODE == m_eDisplayMode )
    {
        // Here stop slide show, and continue normal mode
        m_pSlideShow->Stop();
        m_eDisplayMode = m_eOldDisplayMode;
        CString csCurrentFile = m_pFileExplorer->GetCurrentName();
        LoadImage( csCurrentFile, false );
    }
    else if( BACKGROUND_MODE == m_eDisplayMode || 
             DIALOG_MODE == m_eDisplayMode )
    {
        // Send Mouse LButton down message to other windows.
        for( int nWindowIndex = 0; nWindowIndex < m_Windows.size(); ++nWindowIndex )
        {
            if( m_Windows[nWindowIndex]->OnLButtonDown( point.x, point.y, nFlags ))
            {
                return;
            }
        }
        if( MK_LBUTTON == ( nFlags & MK_LBUTTON ))
        {
            if( BACKGROUND_MODE == m_eDisplayMode )
            {
                // Here switch from 
                SwitchModeToDialog();
            }
            else
            {
                m_bDlgMoveEnabled = true;
                m_ptDlgMoveStart = point;
                SetCapture();
            }
        }
    }
    CDialog::OnLButtonDown(nFlags, point);
}

void CPicZoomDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
    // Send Mouse LButton up message to other windows.
    for( int nWindowIndex = 0; nWindowIndex < m_Windows.size(); ++nWindowIndex )
    {
        if( m_Windows[nWindowIndex]->OnLButtonUp( point.x, point.y, nFlags ))
        {
            return;
        }
    }

    // Disable Dialog movement in pan mode.
    if( m_bDlgMoveEnabled )
    {
        ReleaseCapture();
    }
    m_bDlgMoveEnabled = false;
    CDialog::OnLButtonUp(nFlags, point);
}

void CPicZoomDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
    CAboutDlg Dlg;
    Dlg.DoModal();
    CDialog::OnRButtonDown(nFlags,point);
}

void CPicZoomDlg::SwitchModeToDialog()
{
    int nError = ModifyStyle( 0 , WS_OVERLAPPED | WS_CAPTION | WS_MAXIMIZEBOX | 
                                  WS_MINIMIZEBOX | WS_SYSMENU |WS_THICKFRAME, 0 );

    RemoveFromWindowList( L"BackGround" );

    ImageArea* pImage = (ImageArea*)GetFromWindowList( L"ImageArea" );

    int nImageWidth = 0;
    int nImageHeight = 0;

    if( pImage )
    {
         pImage->GetImageArea( nImageWidth, nImageHeight );
    }
    int nTitleBarHeight = ::GetSystemMetrics( SM_CYSIZE );
    if( nImageWidth < 150 )
    {
        nImageWidth = 150;
    }
    if( nImageHeight < 150 )
    {
        nImageHeight = 150;
    }
    int nLeft = ( m_Desktop.right - nImageWidth - 20 - 10 ) / 2;
    int nTop = ( m_Desktop.bottom - nImageHeight - 20 - nTitleBarHeight - 10) / 2;
    int nWidth = nImageWidth + 40;
    int nHeight = nImageHeight + 40;

    // Get Translattion from ImageArea.
    int nXTranslate = 0;
    int nYTranslate = 0;
    if( pImage )
    {
        pImage->GetTranslation( nXTranslate, nYTranslate );

        // Avoid Translatoin in Dialog mode.
        pImage->SetTranslation( 0, 0 );
    }

    SetWindowPos( 0, nLeft + nXTranslate, nTop + nYTranslate, nWidth, nHeight + nTitleBarHeight, 0 );
    Invalidate( true );

    RECT ClientArea;
    GetClientRect( &ClientArea );
    ChangeWindowSize( ClientArea.right, ClientArea.bottom );

    m_DlgRegion = CRect( nLeft, nTop, nLeft + nWidth, nTop + nHeight );


    CString csWindowTitle;
    csWindowTitle.Format( L"%s - PicZoom Photo Viewer", m_csFileName );
    SetWindowText( csWindowTitle );

    m_eDisplayMode = DIALOG_MODE;
    Invalidate( false );
}

void CPicZoomDlg::SwitchModeToBackground()
{
    int nError = ModifyStyle(WS_OVERLAPPED | WS_CAPTION | WS_MAXIMIZEBOX | 
        WS_MINIMIZEBOX | WS_SYSMENU |WS_THICKFRAME, 0, 0 );
    ShowWindow( SW_HIDE );
    SetWindowText( L"PicZoom Photo Viewer" );

    // Take screen capture.
    m_ScreenCapture.TakeScreenShot();

    // Change Window size to Desktop size.
    SetWindowPos( 0, 0, 0, m_Desktop.right, m_Desktop.bottom, SWP_SHOWWINDOW );
    (CoordConverter::GetInstance()).SetWindowRegion( m_Desktop.right, m_Desktop.bottom );

    // Set new region to all Windows, and enable redraw flag.
    for( int nWindowIndex = m_Windows.size() - 1; nWindowIndex >= 0 ; --nWindowIndex )
    {
        m_Windows[nWindowIndex]->SetRegion( m_Desktop.right, m_Desktop.bottom );
    }


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    int nWidth = (float)m_Desktop.right / 2;
    int nHeight = (float)m_Desktop.bottom/ 2;
    glOrtho( -nWidth, nWidth, -nHeight, nHeight, -1.0, 5.0 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glViewport( 0, 0, m_Desktop.right, m_Desktop.bottom );
    glGetIntegerv( GL_VIEWPORT, viewport );
    m_eDisplayMode = BACKGROUND_MODE;

    // Change size of Display area.
    m_nDisplayWidth = m_Desktop.right;
    m_nDisplayHeight = m_Desktop.bottom;

    // Create Background object and add to vector.
    BackGround* pNewObj = new BackGround( m_hWnd );
    pNewObj->SetVisibility( true );
    pNewObj->SetScreenCaptureObj( &m_ScreenCapture );
    pNewObj->SetRegion( m_Desktop.right, m_Desktop.bottom );
    if( !pNewObj->SetupWindow())
    {
        // Background texture creation failed.
        // Display is not Ok :(
        exit( 0 );
    }
    m_Windows.push_back( pNewObj );

    // Enable display of Bottom window.
    m_bDisplayBottomWindow = true;
    Invalidate( false );
}

void CPicZoomDlg::ChangeWindowSize( const int nWidth_i, const int nHeight_i )
{
    // Change size of Display area.
    m_nDisplayWidth = nWidth_i;
    m_nDisplayHeight = nHeight_i;

    (CoordConverter::GetInstance()).SetWindowRegion( nWidth_i, nHeight_i );
    // Set new region to all Windows, and enable redraw flag.
    for( int nWindowIndex = m_Windows.size() - 1; nWindowIndex >= 0 ; --nWindowIndex )
    {
        m_Windows[nWindowIndex]->SetRegion( nWidth_i, nHeight_i );
    }
    m_bDisplayBottomWindow = true;


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    int nWidthProj = (float)nWidth_i / 2;
    int nHeightProj = (float)nHeight_i / 2;
    glOrtho( -nWidthProj, nWidthProj, -nHeightProj, nHeightProj, -1.0, 1.0 );


    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glViewport( 0, 0, nWidth_i, nHeight_i );
}

BOOL CPicZoomDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg)
{
    HCURSOR hActiveCursor = (CursorManager::GetInstance())->GetCurrentCursor();
    if( (CursorManager::GetInstance())->GetCursor( CURSOR_NORMAL_TYPE ) != hActiveCursor )
    {
        SetCursor( hActiveCursor );
    }
    else
    {
        CDialog::OnSetCursor( pWnd, nHitTest, msg );
    }
    return true;
}

BOOL CPicZoomDlg::PreTranslateMessage(MSG* pMsg)
{
    if( WM_ERASEBKGND == pMsg->message )
    {
        return true;
    }
    if( WM_NCMOUSEMOVE == pMsg->message )
    {
        // Need to change the cursor.
        (CursorManager::GetInstance())->SetActiveCursor( CURSOR_NORMAL_TYPE );
    }
    if( WM_KEYDOWN == pMsg->message )
    {
        // Comment from Sujith. I never used keyboard functionalities 
        // of Picasa Image Viewer. He found that File opening and Zoom can be handled with
        // keyboard also.
        if( VK_LEFT == pMsg->wParam )
        {
            OnPreviousFileOpen( 0, 0 );
        }
        else if ( VK_RIGHT == pMsg->wParam )
        {
            OnNextFileOpen( 0, 0 );
        }
        else if ( VK_UP == pMsg->wParam )
        {
            OnMouseWheel( 0, WHEEL_DELTA, CPoint( 0,0 ));
        }
        else if ( VK_DOWN == pMsg->wParam )
        {
            OnMouseWheel( 0, -WHEEL_DELTA, CPoint( 0,0 ));
        }
    }
    return CDialog::PreTranslateMessage( pMsg );
}

LRESULT  CPicZoomDlg::OnDropFiles(WPARAM wParam,LPARAM lParam)
{
    m_bStartupLogoDisplay = false;
    TCHAR	szDroppedFile[1024];
    memset( szDroppedFile, 0, sizeof( szDroppedFile ));
    HDROP	hDrop ;
    int nFiles;
    CString csString;

    hDrop = (HDROP)wParam;
    nFiles = DragQueryFile(hDrop,		// Struture Identifier
        0,			// -1 to Drop more than one file
        szDroppedFile,// Droped File Name
        1023 *2 );	// Max char
    m_nRotate = 0;
    LoadImage( szDroppedFile );
    Invalidate( false );
    m_pFileExplorer->SetFilePath( szDroppedFile );
    return 0;
}

void CPicZoomDlg::LoadImage( LPCTSTR pImagePath_i, bool bNeedToDesktopCapture_i )
{
    // Delete old image from WindowList.
    RemoveFromWindowList( L"ImageArea" );

    ImageArea* pNewImage = new ImageArea( m_hWnd, m_pFontEngine );
	/*
    if( m_bStartupLogoDisplay )
    {
        pNewImage->SetImageResourceID( IDB_STARTUP_LOGO );
    }
    else
    {
        pNewImage->SetImageFilePath( pImagePath_i );
    }*/

	pNewImage->SetImageFilePath(pImagePath_i);

    if( bNeedToDesktopCapture_i )
    {
        pNewImage->SetRegion( m_Desktop.right, m_Desktop.bottom );
    }
    else
    {
        pNewImage->SetRegion( m_nDisplayWidth, m_nDisplayHeight );
    }
    pNewImage->SetBackgroundCapture( bNeedToDesktopCapture_i );
    pNewImage->SetupWindow();

    // Inorder to keep background images drawn at first, we need to pop it.
    GLWindowBase* pBackground = (GLWindowBase*)GetFromWindowList( L"BackGround" );
    if( pBackground )
    {
        m_Windows.pop_back();
    }

    m_Windows.push_back( pNewImage );
    if(pBackground)
    {
        m_Windows.push_back( pBackground );
    }
    if( bNeedToDesktopCapture_i )
    {
        SwitchModeToBackground();
    }

    // Get File name;
    try
    {
        CFile File;
        if( File.Open( pImagePath_i, CFile::modeRead ))
        {
            m_csFileName = File.GetFileName();
        }
    }
    catch( ... )
    {
        AfxMessageBox( L"File Not found:"+CString( pImagePath_i ));
    }

    // In dialog mode, change window name
    if( DIALOG_MODE == m_eDisplayMode )
    {
        CString csWindowTitle;
        csWindowTitle.Format( L"%s - PicZoom Photo Viewer", m_csFileName );
        SetWindowText( csWindowTitle );
    }
}

LRESULT CPicZoomDlg::OnZoomPlus( WPARAM wParam, LPARAM lParam )
{
    if( 1 == wParam )
    {
        ImageArea* pImage = (ImageArea*)GetFromWindowList( L"ImageArea" );
        pImage->ZoomPlus( false );
    }
    else
    {
        ImageArea* pImage = (ImageArea*)GetFromWindowList( L"ImageArea" );
        pImage->ZoomPlus( true );
    }

    return 1;
}
LRESULT CPicZoomDlg::OnZoomMinus( WPARAM wParam, LPARAM lParam )
{
    if( 1 == wParam )
    {
        ImageArea* pImage = (ImageArea*)GetFromWindowList( L"ImageArea" );
        pImage->ZoomMin( false );
    }
    else
    {
        // Action start.
        ImageArea* pImage = (ImageArea*)GetFromWindowList( L"ImageArea" );
        pImage->ZoomMin( true );
    }
    return 1;
}
LRESULT CPicZoomDlg::OnZoomOneToOne( WPARAM wParam, LPARAM lParam )
{
    if( 0 == wParam )
    {
        ImageArea* pImage = (ImageArea*)GetFromWindowList( L"ImageArea" );
        pImage->ZoomOneToOne();
    }
    return 1;
}

LRESULT CPicZoomDlg::OnRotateClockWise( WPARAM wParam, LPARAM lParam )
{
    if( 0 == wParam )
    {
        ImageArea* pImage = (ImageArea*)GetFromWindowList( L"ImageArea" );
        pImage->RotateClockWise();
    }
    Draw();
    return 1;
}
LRESULT CPicZoomDlg::OnRotateAntiClockWise( WPARAM wParam, LPARAM lParam )
{
    if( 0 == wParam )
    {
        ImageArea* pImage = (ImageArea*)GetFromWindowList( L"ImageArea" );
        pImage->RotateAntiClockWise();
    }
    Draw();
    return 1;
}

LRESULT CPicZoomDlg::OnNextFileOpen( WPARAM wParam, LPARAM lParam )
{
    if( 0 == wParam )
    {
        CString csNextFile = m_pFileExplorer->GetNextFileName();
        if( 0 == csNextFile.GetLength() )
        {
            BottomWindows* pBottomWindow = (BottomWindows*)GetFromWindowList( L"BottomWindows" );
            // Set Error message
            pBottomWindow->SetMessage( L"No image file loaded. Drag an image to PicZoom." );
        }
        if( 0 != csNextFile.Compare( m_csFileURL ))
        {
            LoadImage( csNextFile, false );
            m_csFileURL = csNextFile;
            Draw();
        }
    }
    return 1;
}
LRESULT CPicZoomDlg::OnPreviousFileOpen( WPARAM wParam, LPARAM lParam )
{
    if( 0 == wParam )
    {
        CString csNextFile = m_pFileExplorer->GetPreviousFileName();
        if( 0 == csNextFile.GetLength() )
        {
            BottomWindows* pBottomWindow = (BottomWindows*)GetFromWindowList( L"BottomWindows" );
            // Set Error message
            pBottomWindow->SetMessage( L"No image file loaded. Drag an image to PicZoom." );
        }
        if( 0 != csNextFile.Compare( m_csFileURL ))
        {
            m_nRotate = 0;
            LoadImage( csNextFile, false );
            m_csFileURL = csNextFile;
            Draw();
        }
    }
    return 1;
}

LRESULT  CPicZoomDlg::OnSlideShow( WPARAM wParam, LPARAM lParam )
{
    if( 0 == wParam )
    {
        if( SLIDE_SHOW_MODE == m_eDisplayMode )
        {
            // Already slide show started.
            return 1;
        }
        if( m_bStartupLogoDisplay )
        {
            BottomWindows* pBottomWindow = (BottomWindows*)GetFromWindowList( L"BottomWindows" );
            // Set Error message
            pBottomWindow->SetMessage( L"No image file loaded. Drag an image to PicZoom." );
            // When startup logo is displayed, Slide show is disabled.
            return 1;
        }
        // Enable Slideshow mode.
        m_eOldDisplayMode = m_eDisplayMode;
        m_eDisplayMode = SLIDE_SHOW_MODE;
        m_pSlideShow = new SlideShow( m_hWnd, m_pFileExplorer );
        m_pSlideShow->Start();
    }
    return 1;
}

LRESULT  CPicZoomDlg::OnMakeToCenter( WPARAM wParam, LPARAM lParam )
{
    if( 0 == wParam )
    {
        ImageArea* pImage = (ImageArea*)GetFromWindowList( L"ImageArea" );
        pImage->MakeToCenter();
    }
    return 1;
}

LRESULT  CPicZoomDlg::OnNewFileLoad( WPARAM wParam, LPARAM lParam )
{

    // Stop logo display after first image request.
    m_bStartupLogoDisplay = false;

    TCHAR* ptcFileName = (TCHAR*)lParam;
    CString csFileName = CString( ptcFileName );

    m_csFileURL = csFileName.Left( csFileName.GetLength() - 1);
    m_csFileURL = m_csFileURL.Right( m_csFileURL.GetLength() - 1);
    LoadImage( m_csFileURL );
    m_pFileExplorer->SetFilePath( m_csFileURL );
    return 1;
}


void CPicZoomDlg::OnCancel()
{
    OnClose();
    CDialog::OnCancel();
}

void CPicZoomDlg::OnOK()
{
    OnClose();
    CDialog::OnOK();
}

void CPicZoomDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize( nType, cx, cy );
    // Width and height is checked with 0, to avoid issues in Minimize state.
    if( false == m_bInitComplete || 0 == cx || 0 == cy )
    {
        return;
    }
    if( DIALOG_MODE == m_eDisplayMode )
    {
        CRect ClientArea;
        // Here change viewport, sometimes the viewport is not correct.
        GetClientRect( &ClientArea );
        ChangeWindowSize( ClientArea.Width(), ClientArea.Height());
        
        // Get Window Area in screen coordinate, to change the window position.
        RECT SreenCoord;
        GetWindowRect( &SreenCoord );
        m_DlgRegion = SreenCoord;
        m_DlgRegion.bottom -= ::GetSystemMetrics( SM_CYSIZE );
    }
}

void CPicZoomDlg::RemoveFromWindowList( CString csWindowName_i )
{
    std::vector<GLWindowBase*>::iterator Iterator;
    for( Iterator = m_Windows.begin(); Iterator != m_Windows.end(); Iterator++ )
    {
        GLWindowBase* pObj = *Iterator;
        CString csWindowName = pObj->GetName();
        if( 0 == csWindowName.Compare( csWindowName_i ) )
        {
            // Remove background object from queue.
            m_Windows.erase( Iterator );

            // Delete BBackground object.
            delete pObj;
            break;
        }
    }
}

GLWindowBase* CPicZoomDlg::GetFromWindowList( CString csWindowName_i )
{
    std::vector<GLWindowBase*>::iterator Iterator;
    for( Iterator = m_Windows.begin(); Iterator != m_Windows.end(); Iterator++ )
    {
        GLWindowBase* pObj = *Iterator;
        CString csWindowName = pObj->GetName();
        if( 0 == csWindowName.Compare( csWindowName_i ) )
        {
            return pObj;
        }
    }
    return 0;
}
