#include "StdAfx.h"
#include "ImageArea.h"
#include "GLTexture.h"
#include "GLVertexBuffer.h"
#include "GLExtension.h"
#include "PicZoomUtil.h"
#include "BMPLoader.h"
#include "FontEngine.h"
#include "ZoomText.h"
#include "UnProject.h"
#include "CursorManager.h"
#include "Consts.h"
#include "GL\gl.h"

#define ZOOM_TIMER_FACTOR 20.0f

ImageArea::ImageArea( HWND hParent, FontEngine* pFontEngine_i ):
GLWindowBase( hParent ),
m_pVertexBuffer( 0 ),
m_pImage( 0 )
{
    SetName( L"ImageArea" );
    m_fXTranslation = 0.0;
    m_fYTranslation = 0.0;
    m_fXTranslationOffSet = 0.0;
    m_fYTranslationOffSet = 0.0;
    m_nWidth = 0;
    m_nHeight = 0;
    m_fZoom = 1.0;
    m_fZoomOffset = 0.0;
    m_nRotate = 0;
    m_bPanEnabled = false;
    m_MouseMoveStart.x = 0;
    m_MouseMoveStart.y = 0;
    m_nResourceID = 0;
    m_pVertexBuffer = new GLVertexBuffer();
    m_pVertexBuffer->Create( 4 );
    m_pZoomText = new ZoomText( pFontEngine_i, m_hParentWindow );
    m_pUnProject = new UnProject( m_fZoom, m_fXTranslation, m_fYTranslation, m_nRotate );
    m_bContinousZoom = false;
}

ImageArea::~ImageArea()
{
    if( m_pUnProject )
    {
        delete m_pUnProject;
        m_pUnProject = 0;
    }
    if( m_pZoomText )
    {
        delete m_pZoomText;
        m_pZoomText = 0;
    }
    if( m_pVertexBuffer )
    {
        delete m_pVertexBuffer;
        m_pVertexBuffer = 0;
    }
}


void ImageArea::SetImageResourceID( const int nResourceID_i )
{
    m_nResourceID = nResourceID_i;
}


void ImageArea::SetImageFilePath( CString csFilePath_i )
{
    m_csFilePath = csFilePath_i;
}


bool ImageArea::SetupWindow()
{
    BMPLoader Loader;
    BYTE* pbyBitmapData = 0;
    if( m_nResourceID ) 
    {
        if( !Loader.LoadBMP( m_nResourceID, m_nImageWidth, m_nImageHeight, pbyBitmapData ))
        {
            return false;
        }
    }
    else
    {
        if( !Loader.LoadBMP( m_csFilePath, m_nImageWidth, m_nImageHeight, pbyBitmapData ))
        {
            return false;
        }
    }
    if( m_pImage )
    {
        delete m_pImage;
        m_pImage = 0;
    }
    m_pImage = new GLTexture;
    if( !m_pImage->Create( m_nImageWidth, m_nImageHeight, pbyBitmapData ))
    {
        return false;
    }
    delete[] pbyBitmapData;

    // Setup Vertex buffer and UnProject.
    int nHalfOfImageY = m_nImageHeight / 2;
    int nHalfOfImageX = m_nImageWidth / 2;
    /*
    0--3
    |  |
    1--2
    */

    float fTextureXMax = 1.0;
    float fTextureYMax = 1.0;
    if( !GLExtension::GetInstance().m_bNonPowerOfTwo )
    {
        fTextureXMax = (float)m_nImageWidth / PicZoomUtil::GetNearestPowerOf2( m_nImageWidth );
        fTextureYMax = (float)m_nImageHeight / PicZoomUtil::GetNearestPowerOf2( m_nImageHeight );
    }

    m_pVertexBuffer->SetAt( 0, -nHalfOfImageX ,  nHalfOfImageY, 0.0f, 0.0f,         fTextureYMax); // Left Top  corner
    m_pVertexBuffer->SetAt( 1, -nHalfOfImageX , -nHalfOfImageY, 0.0f, 0.0f,         0.0f), // Left Bottom
    m_pVertexBuffer->SetAt( 2, nHalfOfImageX  , -nHalfOfImageY, 0.0f, fTextureXMax, 0.0f); // Right bottom
    m_pVertexBuffer->SetAt( 3, nHalfOfImageX  ,  nHalfOfImageY, 0.0f, fTextureXMax, fTextureYMax); // Right top

    // Set image area to UnProject instance, this values are used for identifying image position.
    m_pUnProject->SetImageArea( -nHalfOfImageX, -nHalfOfImageY, m_nImageWidth, m_nImageHeight );

    // Zoom handling
    m_ZoomTimer.SetMaxElapseTime( 10 );
    m_fZoomOffset = 1.0 / 10.0;
    m_fZoom = 0.0;

    PrepareZoomDisplay();

    return true;
}

/// Drawing of Image.
bool ImageArea::Draw()
{
    ZoomHandling();
    TranslateHandling();
    RotateHandling();

    m_pImage->Enable();
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    // Y value is -ve, only becouse opengl Y coordinate is increasing from bottom to top,
    // But Y direction mouse movement received in Dialog is decreasing from bottom to top.
    glTranslatef( m_fXTranslation, -m_fYTranslation, 0.0 );
    glScalef( m_fZoom, m_fZoom, 1.0 );
    glRotatef( ROTATION_ANGLES[m_nRotate], 0, 0, 1 );

    m_pVertexBuffer->DrawVertexBuffer( GL_QUADS );

    // After image drawing, drawing Zoom text
    m_pZoomText->Draw();

    return true;
}

void ImageArea::ZoomHandling()
{
    if( m_ZoomTimer.IsEnabled() || m_bContinousZoom )
    {
        if( m_ZoomTimer.IsEnabled())
        {
            m_ZoomTimer.ElapseTime();
            // Zoom on to off
            if( !m_ZoomTimer.IsEnabled())
            {
                /*
                // When Zoom completed, set visibility of bottom windows to true
                if( m_bDisplayBottomWindow )
                {
                    // Display Bottom windows for first time.
                    m_pBottomWindows->SetVisibility( true );
                    m_bDisplayBottomWindow = false;
                }*/
            }
        }
        m_fZoom += ( m_fZoomOffset );
        if( !m_bContinousZoom )
        {
            //m_fZoomOffset -= ( m_fZoomOffset );
        }
        if( m_fZoom < 0.01 )
        {
            m_fZoom = 0.01;
        }
        m_pZoomText->SetZoomValue( m_fZoom );

        int nOffSet =1;
        if( m_fZoomOffset < 0.0 )
        {
            nOffSet = -1;
        }
    int nHalfOfImageY = m_nImageHeight / 2;
    int nHalfOfImageX = m_nImageWidth / 2;
    /*
    0--3
    |  |
    1--2
    */
    /*
    GLVertex VertexData[4];
    // Here No Rotation.
    VertexData[0] = m_pVertexBuffer->Get( 0 );
    VertexData[1] = m_pVertexBuffer->Get( 1 );
    VertexData[2] = m_pVertexBuffer->Get( 2 );
    VertexData[3] = m_pVertexBuffer->Get( 3 );

    m_pVertexBuffer->SetAt( 0, VertexData[0].x - nOffSet, VertexData[0].y + nOffSet, 0.0f, 0.0f,1.0f); // Left Top  corner
    m_pVertexBuffer->SetAt( 1, VertexData[1].x - nOffSet, VertexData[1].y - nOffSet, 0.0f, 0.0f,0.0f), // Left Bottom
    m_pVertexBuffer->SetAt( 2, VertexData[2].x + nOffSet, VertexData[2].y - nOffSet, 0.0f, 1.0f,0.0f); // Right bottom
    m_pVertexBuffer->SetAt( 3, VertexData[3].x + nOffSet, VertexData[3].y + nOffSet,  0.0f, 1.0f,1.0f); // Right top
*/
    }
    else
    {
        m_fZoomOffset = 0.0;
        ::KillTimer( m_hParentWindow, TIMER_ZOOM );
    }
}

void ImageArea::TranslateHandling()
{
    if( m_TranslateTimer.IsEnabled())
    {
        m_TranslateTimer.ElapseTime();
        if( 0 != m_fXTranslationOffSet )
        {
            m_fXTranslation += m_fXTranslationOffSet / 2.0;
            m_fXTranslationOffSet += -1 * ( m_fXTranslationOffSet / 2.0);
        }
        if( 0 != m_fYTranslationOffSet )
        {
            m_fYTranslation += m_fYTranslationOffSet / 2.0;
            m_fYTranslationOffSet += -1 * (m_fYTranslationOffSet / 2.0);
        }
    }
    else
    {
        m_fYTranslationOffSet = 0;
        m_fXTranslationOffSet = 0;
        if( !m_bPanEnabled )
        {
            ::KillTimer( m_hParentWindow, TIMER_TRANSLATE );
        }
    }
}

void ImageArea::RotateHandling()
{
    if( m_RotateTimer.IsEnabled() )
    {
        m_RotateTimer.ElapseTime();
        --m_nRotate;
        if( 0 == m_nRotate )
        {
            ::KillTimer( m_hParentWindow, TIMER_MAKE_TO_CENTER );
        }
        // Make rotate index to 0.
        max( 0, m_nRotate );
    }
}

bool ImageArea::OnMouseWheel( const int nX_I, const int nY_i, const UINT nFlags, const short zDelta )
{
    float fZoomValue = float( zDelta ) / WHEEL_DELTA;

    // Subin's comment, restrict Zoom to a maximum value.
    // Then its okay, I blocked zoom event after 3000.
    if( m_fZoom > ZOOM_MAX_VALUE && fZoomValue > 0.0 )
    {
        return true;
    }

    if( 0 == m_fZoomOffset )
    {
        ::SetTimer( m_hParentWindow, TIMER_ZOOM, 5, 0 );
    }
    
    // Findout zoom factor based on width of and height of image.
    if( m_nImageWidth > m_nWidth || m_nImageHeight > m_nHeight )
    {
        float fImageToDesktopRatioX = (float)m_nWidth / m_nImageWidth;
        float fImageToDesktopRatioY = (float)m_nHeight / m_nImageHeight;
        float fImageToDesktopRatio = min( fImageToDesktopRatioY, fImageToDesktopRatioX );
        fZoomValue = fZoomValue * fImageToDesktopRatio;
    }

    m_fZoomOffset += ( fZoomValue / 100 );
    // Apply Zoom factor 15 times. then first single scroll make 15% zoom.
    m_ZoomTimer.SetMaxElapseTime( 15 );
    return true;
}

void ImageArea::PrepareZoomDisplay()
{
    int nTimerCount = m_bDesktopCaptured ? 10 : 1;
    m_fZoom = 0.0;
    // Make Image Fit into the Window region.
    if( m_nImageWidth > ( m_nWidth - 40 ) || m_nImageHeight > ( m_nHeight + 40 ) )
    {
        // Find out Zoom factor.
        float fXDirZoom = ( m_nWidth - 40 ) / float( m_nImageWidth );
        float fYDirZoom = ( m_nHeight - 40 ) / float( m_nImageHeight );
        m_fZoomOffset = ( min( fXDirZoom, fYDirZoom )) / float( nTimerCount );
        m_ZoomTimer.SetMaxElapseTime( nTimerCount );
        ::SetTimer( m_hParentWindow, TIMER_ZOOM, 10, 0 );
    }
    else
    {
        m_fZoomOffset = 1.0 / nTimerCount;
        m_ZoomTimer.SetMaxElapseTime( nTimerCount );
        ::SetTimer( m_hParentWindow, TIMER_ZOOM, 10, 0 );
    }
}

bool ImageArea::ZoomPlus( const bool bOn_i )
{
    if( bOn_i )
    {
        // Action start
        if( 0 == m_fZoomOffset )
        {
            ::SetTimer( m_hParentWindow, TIMER_ZOOM, 10, 0 );
        }
        m_fZoomOffset += 1.0 / 100.0;
//        m_ZoomTimer.SetMaxElapseTime( 1 );
        m_bContinousZoom = true;
    }
    else
    {
        // Off zoom timer. After infinity off, one frame will be displayed.
        //m_ZoomTimer.EnableInfinity( false );
        m_bContinousZoom = false;
    }
    return true;
}
bool ImageArea::ZoomMin( const bool bOn_i )
{
    if( bOn_i )
    {
        // Action start
        if( 0 == m_fZoomOffset )
        {
            ::SetTimer( m_hParentWindow, TIMER_ZOOM, 10, 0 );
        }
        m_fZoomOffset -= ( ( 2.5 / 100.0 ) ) / 2;
        //m_ZoomTimer.SetMaxElapseTime( 1 );
        //m_ZoomTimer.EnableInfinity( true );
        m_bContinousZoom = true;
    }
    else
    {
        // Off zoom timer. After infinity off, one frame will be displayed.
       // m_ZoomTimer.EnableInfinity( false );
        m_bContinousZoom = false;
    }
    return true;
}
bool ImageArea::ZoomOneToOne()
{
    ::SetTimer( m_hParentWindow, TIMER_ZOOM, 10, 0 );
    m_fZoomOffset = ( 1.0 - m_fZoom ) / 10.0;
    m_ZoomTimer.SetMaxElapseTime( 10 );
    return true;
}
bool ImageArea::MakeToCenter()
{
    // Here find the Zoom to make the image Center.
    m_fXTranslationOffSet = -m_fXTranslation;
    m_fYTranslationOffSet = -m_fYTranslation;
    m_TranslateTimer.SetMaxElapseTime( 10 );
    ::SetTimer( m_hParentWindow, TIMER_TRANSLATE, 10, 0 );

    // When Zoom is applied, avoid the unwanted zoom.
    // Find zoom factor for image display.
    float fXDirZoom = m_nWidth / (float)m_nImageWidth;
    float fYDirZoom = m_nHeight / (float)m_nImageHeight;
    // Get minimum zoom, that can be used for displaying full screen Drawing.
    float fExpectedZoom = min( fXDirZoom, fYDirZoom );

    // When High zoom is possible, then avoid the zoom.
    fExpectedZoom = min( 1.0, fExpectedZoom );
    m_fZoomOffset = -( m_fZoom - fExpectedZoom );
    if( m_fZoomOffset < 0.0 || m_fZoomOffset > 0.0 )
    {
        m_fZoomOffset = m_fZoomOffset / 10.0;
        m_ZoomTimer.SetMaxElapseTime( 10 );
        ::SetTimer( m_hParentWindow, TIMER_ZOOM, 10, 0 );
    }

    // When image is in rotated state, then make to original state.
    // This rotation is set only if translation is applied, 
    // else make to non-rotated image.
    if( m_nRotate && ( m_fYTranslationOffSet > 0.0 || m_fXTranslationOffSet > 0.0 ))
    {
        // Here start rotation,
        m_RotateTimer.SetMaxElapseTime( m_nRotate );
        // Enable timer to start rotation.
        ::SetTimer( m_hParentWindow, TIMER_MAKE_TO_CENTER, 10, 0 );
    }
    else
    {
        m_nRotate = 0;
    }

    return true;
}

bool ImageArea::RotateAntiClockWise()
{
    m_nRotate--;
    if( m_nRotate < 0 )
    {
        m_nRotate = 3;
    }
    return true;
}

bool ImageArea::RotateClockWise()
{
    m_nRotate++;
    if( m_nRotate >= 4 )
    {
        m_nRotate = 0;
    }
    return true;
}

void ImageArea::GetImageArea( int& nImageWidth_o, int& nImageHeight_o )
{
    nImageWidth_o = m_nImageWidth * m_fZoom;
    nImageHeight_o = m_nImageHeight * m_fZoom;
}

void ImageArea::GetTranslation( int& nXTranslation_o, int& nYTranslation_o )
{
    nXTranslation_o = m_fXTranslation;
    nYTranslation_o = m_fYTranslation;
}
void ImageArea::SetTranslation( const int nXTranslation_i, const int nYTranslation_i )
{
    m_fXTranslation = nXTranslation_i;
    m_fYTranslation = nYTranslation_i;
}
bool ImageArea::IsWithinImageArea( const int nX_i, const int nY_i )
{
    return m_pUnProject->IsWithinRange( nX_i, nY_i );
}

bool ImageArea::OnLButtonDown( const int nX_i, const int nY_i, const int nFlags_i )
{
    if( m_pUnProject->IsWithinRange( nX_i, nY_i ))
    {
        if( !m_bPanEnabled )
        {
            ::SetTimer( m_hParentWindow, TIMER_TRANSLATE, 16, 0 );
            m_MouseMoveStart.x = nX_i;
            m_MouseMoveStart.y = nY_i;

            // ::ScreenToClient( m_MouseMoveStart );
            m_bPanEnabled = true;
        }
        HCURSOR hHandCursor = CursorManager::GetInstance()->GetCursor( CURSOR_HAND_TYPE );
        CursorManager::GetInstance()->SetActiveCursor( CURSOR_HAND_TYPE );
        ::SetCursor( hHandCursor );
        return true;
    }
    return false;
}

bool ImageArea::OnLButtonUp( const int nX_i, const int nY_i, const int nFlags_i )
{
    if( CursorManager::GetInstance()->GetCurrentCursor() == CursorManager::GetInstance()->GetCursor( CURSOR_HAND_TYPE ) )
    {
        HCURSOR hMoveCursor = CursorManager::GetInstance()->GetCursor( CURSOR_MOVE_TYPE );
        CursorManager::GetInstance()->SetActiveCursor( CURSOR_MOVE_TYPE );
        ::SetCursor( hMoveCursor );
    }
    m_bPanEnabled = false;

    // Nothing handled in Button up. Should transfer LButton up to other windows.
    return false;
}

bool ImageArea::OnMouseMove( const int nX_i, const int nY_i, const int nFlags_i )
{
    bool bReturnValue = false;
    if( MK_LBUTTON == ( nFlags_i & MK_LBUTTON ))
    {
        if( m_bPanEnabled )
        {
            POINT CursorPos;
            GetCursorPos( &CursorPos );
            ::ScreenToClient( m_hParentWindow, &CursorPos );
            POINT point = CursorPos;
            m_fXTranslationOffSet += ( point.x - m_MouseMoveStart.x );
            m_fYTranslationOffSet += ( point.y - m_MouseMoveStart.y );
            m_TranslateTimer.SetMaxElapseTime( 4, true );
            m_MouseMoveStart = point;
            (CursorManager::GetInstance())->SetActiveCursor( CURSOR_HAND_TYPE );
            bReturnValue = true;
        }
    }
    else
    {
        if( m_pUnProject->IsWithinRange( nX_i, nY_i ))
        {
            (CursorManager::GetInstance())->SetActiveCursor( CURSOR_MOVE_TYPE );
            HCURSOR hCur = (CursorManager::GetInstance())->GetCurrentCursor();
            SetCursor( hCur );
            bReturnValue = true;
        }
        else
        {
            (CursorManager::GetInstance())->SetActiveCursor( CURSOR_NORMAL_TYPE );
            HCURSOR hCur = (CursorManager::GetInstance())->GetCurrentCursor();
            SetCursor( hCur );
            bReturnValue = false;
        }
    }
    return bReturnValue;
}