#include "StdAfx.h"
#include "BottomWindows.h"
#include "GLButton.h"
#include "GLText.h"
#include "PlayButton.h"
#include "FontEngine.h"
#include "resource.h"
#include "Consts.h"
#include "CursorManager.h"
#include "string.h"
#include "GL\gl.h"

// Width and Height of Different windows
struct BUTTON_SIZE_st
{
    int nWidth;
    int nHeight;
};

const BUTTON_SIZE_st stNextSize = { 25, 28 };
const BUTTON_SIZE_st stPreviousSize = { 25, 28 };
const BUTTON_SIZE_st stClockSize = { 28, 19 };
const BUTTON_SIZE_st stAntiClockSize = { 28, 19 };
const BUTTON_SIZE_st stZoom1to1Size = { 28, 19 };
const BUTTON_SIZE_st stZoomMinSize = { 28, 19 };
const BUTTON_SIZE_st stZoomPlusSize = { 29, 19 };
const BUTTON_SIZE_st stMakeCenterSize = { 28, 19 };

const int BOTTOM_WINDOW_WIDTH = 144;

BottomWindows::BottomWindows(HWND hParent_i, FontEngine* pFontEngine_i):
GLWindowBase( hParent_i )
{
    SetName( L"BottomWindows" );
    m_nWidth = m_nHeight = 0;
    m_pButtonZoomMin = new GLButton(hParent_i);
    m_pButtonZoomMax = new GLButton(hParent_i);
    m_pButtonZoomOneToOne = new GLButton(hParent_i);
    m_pButtonNext = new GLButton(hParent_i);
    m_pPlayButton = new PlayButton(hParent_i);
    m_pButtonPrevious = new GLButton(hParent_i);
    m_pButtonRotateCW = new GLButton(hParent_i);
    m_pButtonRotateACW = new GLButton(hParent_i);
    m_pButtonMakeCenter = new GLButton(hParent_i);

    // Add all windows to Window List.
    m_WindowList.push_back( m_pButtonZoomMin );
    m_WindowFunctionality.push_back( CString( L"Zoom Out") );

    m_WindowList.push_back( m_pButtonZoomMax );
    m_WindowFunctionality.push_back( CString( L"Zoom In" ));

    m_WindowList.push_back( m_pButtonZoomOneToOne );
    m_WindowFunctionality.push_back( CString( L"Zoom to 1:1" ));

    m_WindowList.push_back( m_pPlayButton );
    m_WindowFunctionality.push_back( CString( L"Play Slide Show" ));

    m_WindowList.push_back( m_pButtonNext );
    m_WindowFunctionality.push_back( CString( L"Next Photo" ));

    m_WindowList.push_back( m_pButtonPrevious );
    m_WindowFunctionality.push_back( CString( L"Previous Photo" ));

    m_WindowList.push_back( m_pButtonRotateCW );
    m_WindowFunctionality.push_back( CString( L"Rotate Clockwise" ));

    m_WindowList.push_back( m_pButtonRotateACW );
    m_WindowFunctionality.push_back( CString( L"Rotate Counter-Clockwise" ));

    m_WindowList.push_back( m_pButtonMakeCenter );
    m_WindowFunctionality.push_back( CString( L"Make Image To Center" ));

    // Creating GLText for displaying a text with the functionality name of 
    // each button. GLText can display smooth hide/show through alpha blending.
    m_pWindowName = new GLText( m_hParentWindow, pFontEngine_i, TIMER_BOTTOM_WINDOW_VISIBLITY );
    m_bVisible = false;
}

BottomWindows::~BottomWindows(void)
{
    for( int nIndex = 0; nIndex < m_WindowList.size(); nIndex++ )
    {
        delete m_WindowList[nIndex];
    }
    if( m_pWindowName )
    {
        delete m_pWindowName;
        m_pWindowName = 0;
    }
}
bool BottomWindows::Initialize()
{
    m_pButtonZoomMax->SetImage( IDB_ZOOM_PLUS );
    m_pButtonZoomMax->SetLButtonMessage( WM_ZOOM_PLUS_MESSAGE );
    m_pButtonZoomMax->SetRegion( m_nWidth / 2 - 16 - 30 - 30 - 30 - 31 + 3, m_nHeight - 32 + 8,
                                 stZoomPlusSize.nWidth, stZoomPlusSize.nHeight );
    m_pButtonZoomMax->SetupButton();

    m_pButtonZoomMin->SetImage( IDB_ZOOM_MIN );
   // m_pButtonZoomMin->SetVisiblity( true );
    m_pButtonZoomMin->SetLButtonMessage( WM_ZOOM_MIN_MESSAGE );
    m_pButtonZoomMin->SetRegion( m_nWidth / 2 - 16 - 30 - 30 - 30 + 3, m_nHeight - 32 + 8,
                                 stZoomMinSize.nWidth, stZoomMinSize.nHeight );
    m_pButtonZoomMin->SetupButton();

    m_pButtonZoomOneToOne->SetImage( IDB_ONE_TO_ONE );
   // m_pButtonZoomOneToOne->SetVisiblity( true );
    m_pButtonZoomOneToOne->SetLButtonMessage( WM_ZOOM_1TO1_MESSAGE );
    m_pButtonZoomOneToOne->SetRegion( m_nWidth / 2 - 16 - 30 - 30 + 3, m_nHeight - 32+ 8,
                                      stZoom1to1Size.nWidth, stZoom1to1Size.nHeight );
    m_pButtonZoomOneToOne->SetupButton();

    m_pButtonNext->SetImage( IDB_NEXT_BUTTON );
  //  m_pButtonNext->SetVisiblity( true );
    m_pButtonNext->SetLButtonMessage( WM_NEXT_FILE_OPEN );
    m_pButtonNext->SetRegion( m_nWidth / 2 + 16, m_nHeight - 32 + 3, 
                              stNextSize.nWidth, stNextSize.nHeight );
    m_pButtonNext->SetupButton();

    m_pButtonPrevious->SetImage( IDB_PREVIOUS_BUTTON );
 //   m_pButtonPrevious->SetVisiblity( true );
    m_pButtonPrevious->SetLButtonMessage( WM_PREVIOUS_FILE_OPEN );
    m_pButtonPrevious->SetRegion( m_nWidth / 2 - 15 - 25, m_nHeight - 32 + 3, 
                                  stPreviousSize.nWidth, stPreviousSize.nHeight );
    m_pButtonPrevious->SetupButton();

    m_pButtonRotateCW->SetImage( IDB_ROTATE_CLOCK );
  //  m_pButtonRotateCW->SetVisiblity( true );
    m_pButtonRotateCW->SetLButtonMessage( WM_ROTATE_CLOCK_WISE );
    m_pButtonRotateCW->SetRegion( m_nWidth / 2 + 16 + 30, m_nHeight - 32 + 8, 
                                  stClockSize.nWidth, stClockSize.nHeight );
    m_pButtonRotateCW->SetupButton();

    m_pButtonRotateACW->SetImage( IDB_ROTATE_ANTI );
 //   m_pButtonRotateACW->SetVisiblity( true );
    m_pButtonRotateACW->SetLButtonMessage( WM_ROTATE_ANTI_CLOCK_WISE );
    m_pButtonRotateACW->SetRegion( m_nWidth / 2 + 16 + 30 + 30, m_nHeight - 32 + 8,
                                   stAntiClockSize.nWidth, stClockSize.nHeight );
    m_pButtonRotateACW->SetupButton();

    m_pButtonMakeCenter->SetImage( IDB_MAKE_TO_CENTER );
    m_pButtonMakeCenter->SetLButtonMessage( WM_MAKE_TO_CENTER );
    m_pButtonMakeCenter->SetRegion( m_nWidth / 2 + 16 + 30 + 30 + 30, m_nHeight - 32 + 8,
                                    stMakeCenterSize.nWidth, stMakeCenterSize.nHeight );
    m_pButtonMakeCenter->SetupButton();

    m_pPlayButton->SetupButton();
    // m_nWidth / 2 - 40, 40, 62, 68
    
    return true;
}
void BottomWindows::SetRegion( const int nWidth_i, const int nHeight_i )
{
    GLWindowBase::SetRegion( nWidth_i, nHeight_i );
    m_pPlayButton->SetRegion( nWidth_i, nHeight_i );

    // When resize the window, change the size of all other windows.
    m_pButtonZoomMax->SetRegion( m_nWidth / 2 - 16 - 30 - 30 - 30 - 31 + 3, m_nHeight - 32 + 8,
        stZoomPlusSize.nWidth, stZoomPlusSize.nHeight );
    m_pButtonZoomMin->SetRegion( m_nWidth / 2 - 16 - 30 - 30 - 30 + 3, m_nHeight - 32 + 8,
        stZoomMinSize.nWidth, stZoomMinSize.nHeight );
    m_pButtonZoomOneToOne->SetRegion( m_nWidth / 2 - 16 - 30 - 30 + 3, m_nHeight - 32+ 8,
        stZoom1to1Size.nWidth, stZoom1to1Size.nHeight );

    m_pButtonPrevious->SetRegion( m_nWidth / 2 - 15 - 25, m_nHeight - 32 + 3, 
        stPreviousSize.nWidth, stPreviousSize.nHeight );
    m_pButtonNext->SetRegion( m_nWidth / 2 + 16, m_nHeight - 32 + 3, 
        stNextSize.nWidth, stNextSize.nHeight );

    m_pButtonRotateCW->SetRegion( m_nWidth / 2 + 16 + 30, m_nHeight - 32 + 8, 
        stClockSize.nWidth, stClockSize.nHeight );
    m_pButtonRotateACW->SetRegion( m_nWidth / 2 + 16 + 30 + 30, m_nHeight - 32 + 8,
        stAntiClockSize.nWidth, stClockSize.nHeight );
    m_pButtonMakeCenter->SetRegion( m_nWidth / 2 + 16 + 30 + 30 + 30, m_nHeight - 32 + 8,
        stMakeCenterSize.nWidth, stMakeCenterSize.nHeight );
}
bool BottomWindows::OnMouseMove( const int nX_i, const int nY_i, const int nFlags_i )
{
    bool bReturn = false;
    int nMouseOverWindow = -1;
    for( int nIndex = 0; nIndex < m_WindowList.size(); nIndex++ )
    {
        // OnMouseMove Returns mouse over status. When mouse over, that window returns true,
        // and functionality of that window is set as displayText.
        if( (m_WindowList[nIndex])->OnMouseMove( nX_i, nY_i, nFlags_i ))
        {
            nMouseOverWindow = nIndex;
        }
    }
    CString csNewWindowName = L"";
    if( -1 == nMouseOverWindow )
    {
        csNewWindowName = L"";
    }
    else
    {
        csNewWindowName = m_WindowFunctionality[nMouseOverWindow];
    }
    if( m_csMessage.GetLength() == 0 )
    {
        m_pWindowName->SetText( csNewWindowName );
    }
    if( nX_i >= m_nWidth / 2 - BOTTOM_WINDOW_WIDTH && nX_i <= m_nWidth / 2 + BOTTOM_WINDOW_WIDTH &&
        nY_i >= m_nHeight - 32 && nY_i <= m_nHeight )
    {
        if( !m_bVisible && ( ( nFlags_i & MK_LBUTTON ) == 0 ))
        {
            // When mouse is over the bottom window area, then display the window.
            // L button status is checked, when moving the object through the bottom window area,
            // then display of bottom window is not required.
            SetVisibility( true );
        }
        if( -1 == nMouseOverWindow )
        {
            (CursorManager::GetInstance())->SetActiveCursor( CURSOR_NORMAL_TYPE );
            SetCursor( (CursorManager::GetInstance())->GetCurrentCursor() );
        }
        if( m_bVisible )
        {
            // Return true if the mouse movement is within the area of Bottom window.
            return true;
        }
    }
    return false;
}
bool BottomWindows::OnLButtonDown( const int nX_i, const int nY_i, const int nFlags_i )
{
    if(MK_LBUTTON == ( nFlags_i & MK_LBUTTON ))
    {
        for( int nIndex = 0; nIndex < m_WindowList.size(); nIndex++ )
        {
            if((m_WindowList[nIndex])->OnLButtonDown( nX_i, nY_i, nFlags_i ))
            {
                return true;
            }
        }
        if( nX_i >= m_nWidth / 2 - BOTTOM_WINDOW_WIDTH && nX_i <= m_nWidth / 2 + BOTTOM_WINDOW_WIDTH &&
            nY_i >= m_nHeight - 32 && nY_i <= m_nHeight )
        {
            return true;
        }
        else
        {
            // Set bottom windows to hide on mouse down action
            SetVisibility( false );
        }
    }
    return false;
}

bool BottomWindows::OnLButtonUp( const int nX_i, const int nY_i, const int nFlags_i )
{
    for( int nIndex = 0; nIndex < m_WindowList.size(); nIndex++ )
    {
        if( (m_WindowList[nIndex])->OnLButtonUp( nX_i, nY_i, nFlags_i ))
        {
            return true;
        }
    }
    SetVisibility( true );
    return false;
}

void BottomWindows::SetMessage( CString csMessage_i )
{
    m_csMessage = csMessage_i;
    m_pWindowName->SetText( m_csMessage );
}

void BottomWindows::SetVisibility( const bool bVisible_i )
{
    if( m_bVisible != bVisible_i )
    {
        // Set new visibility to all child windows.
        for( int nIndex = 0; nIndex < m_WindowList.size(); nIndex++ )
        {
            (m_WindowList[nIndex])->SetVisibility( bVisible_i );
        }
        m_DrawTimer.SetMaxElapseTime( 100 );
        ::SetTimer( m_hParentWindow, TIMER_BOTTOM_WINDOW_VISIBLITY, 10, 0 );
    }
    m_bVisible = bVisible_i;
}

bool BottomWindows::Draw()
{
    if( !m_bVisible && !m_DrawTimer.IsEnabled() )
    {
        ::KillTimer( m_hParentWindow, TIMER_BOTTOM_WINDOW_VISIBLITY );
        // if Hide, no drawing of bottom windows.
        return true;
    }

    if( !m_DrawTimer.IsEnabled() && !m_pWindowName->IsDrawPending())
    {
        ::KillTimer( m_hParentWindow, TIMER_BOTTOM_WINDOW_VISIBLITY );
        m_csMessage = "";
    }

    int nRemTime = m_DrawTimer.IsEnabled() ? m_DrawTimer.GetRemainingTime() : 100;
    if( m_bVisible && m_DrawTimer.IsEnabled() )
    {
        // When starting, alpha value from low to high
        nRemTime = 100 - nRemTime;
    }
    float fColorComponent =  ( nRemTime / 100.0 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glDisable( GL_TEXTURE_2D );
    glColor4f( 0.5f, 0.5f, 0.5f, 0.4f * fColorComponent );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glPolygonMode( GL_FRONT ,GL_FILL );
    glRectd( -BOTTOM_WINDOW_WIDTH, -m_nHeight / 2 , BOTTOM_WINDOW_WIDTH, -m_nHeight / 2 + 32);
    glDisable( GL_BLEND );
    glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

    for( int nIndex = 0; nIndex < m_WindowList.size(); nIndex++ )
    {
        // Set Transparency for Drawing Windows.
        (m_WindowList[nIndex])->SetTransparency( fColorComponent );
        (m_WindowList[nIndex])->Draw();
    }

    m_DrawTimer.ElapseTime( 4 );

    // Text is drawn only after display window as full color.
    if( !m_DrawTimer.IsEnabled())
    {
        m_pWindowName->Draw( 0, -m_nHeight / 2 + 50 );
    }
    return true;
}
