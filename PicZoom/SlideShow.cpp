#include "StdAfx.h"
#include "SlideShow.h"
#include "GLTexture.h"
#include "GLVertexBuffer.h"
#include "GLExtension.h"
#include "PicZoomUtil.h"
#include "FileExplorer.h"
#include "BMPLoader.h"
#include "Consts.h"

const int ONE_SLIDE_DELAY = 200;

SlideShow::SlideShow( HWND hParent_i, FileExplorer* pFileExplorer_i )
{
    m_hParent = hParent_i;
    m_pFileExplorer = pFileExplorer_i;
    m_pTexture1 = new GLTexture;
    m_pTexture2 = new GLTexture;
    m_pVertexBuffer1 = new GLVertexBuffer;
    m_pVertexBuffer2 = new GLVertexBuffer;
}

SlideShow::~SlideShow(void)
{
}

void SlideShow::Start()
{
    // Reset old projection.
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    // Set Repeat state of FileExplorer to On,
    m_pFileExplorer->SetRepeatState( true );

    RECT FullScreenRegion;
    ::GetWindowRect( ::GetDesktopWindow(), &FullScreenRegion );

    // Here initialize two textures with new current texture and next texture.
    CString csNextFile = m_pFileExplorer->GetCurrentName();
    BMPLoader Loader;
    {
        // Creating first texture with current texture.
        int nWidth, nHeight;
        BYTE* pbyData = 0;
        Loader.LoadBMP( csNextFile, nWidth, nHeight, pbyData );
        if( m_pTexture1 )
        {
            delete m_pTexture1;
            m_pTexture1 = 0;
        }
        m_pTexture1 = new GLTexture();
        m_pTexture1->Create( nWidth, nHeight, pbyData );
        delete[] pbyData;

        float fTextureXMax = 1.0;
        float fTextureYMax = 1.0;
        if( !GLExtension::GetInstance().m_bNonPowerOfTwo )
        {
            fTextureXMax = (float)nWidth / PicZoomUtil::GetNearestPowerOf2( nWidth );
            fTextureYMax = (float)nHeight / PicZoomUtil::GetNearestPowerOf2( nHeight );
        }
        // Create vertex buffer for full region window display.
        m_pVertexBuffer1->Create( 4 );
        m_pVertexBuffer1->SetAt( 0, -nWidth / 2, -nHeight / 2, 0.0, 0.0, 0.0 );
        m_pVertexBuffer1->SetAt( 1, -nWidth / 2,  nHeight / 2, 0.0, 0.0, fTextureYMax );
        m_pVertexBuffer1->SetAt( 2,  nWidth / 2,  nHeight / 2, 0.0, fTextureXMax, fTextureYMax );
        m_pVertexBuffer1->SetAt( 3,  nWidth / 2, -nHeight / 2, 0.0, fTextureXMax, 0.0 );

        // Find zoom factor for full screen window display.
        float fXDirZoom = FullScreenRegion.right / (float)nWidth;
        float fYDirZoom = FullScreenRegion.bottom / (float)nHeight;
        // Get minimum zoom, that can be used for displaying full screen Drawing.
        m_fZoom1 = min( fXDirZoom, fYDirZoom );
    }

    {
        // Creating second texture with next file.
        CString csNextFile = m_pFileExplorer->GetNextFileName();
        BMPLoader Loader;
        int nWidth, nHeight;
        BYTE* pbyData = 0;
        Loader.LoadBMP( csNextFile, nWidth, nHeight, pbyData );

        if( m_pTexture2 )
        {
            delete m_pTexture2;
            m_pTexture2 = 0;
        }
        m_pTexture2 = new GLTexture();
        m_pTexture2->Create( nWidth, nHeight, pbyData );
        delete[] pbyData;

        float fTextureXMax = 1.0;
        float fTextureYMax = 1.0;
        if( !GLExtension::GetInstance().m_bNonPowerOfTwo )
        {
            fTextureXMax = (float)nWidth / PicZoomUtil::GetNearestPowerOf2( nWidth );
            fTextureYMax = (float)nHeight / PicZoomUtil::GetNearestPowerOf2( nHeight );
        }

        // Create vertex buffer for full region window display.
        m_pVertexBuffer2->Create( 4 );
        m_pVertexBuffer2->SetAt( 0, -nWidth / 2, -nHeight / 2, 0.0, 0.0,          0.0 );
        m_pVertexBuffer2->SetAt( 1, -nWidth / 2,  nHeight / 2, 0.0, 0.0,          fTextureYMax );
        m_pVertexBuffer2->SetAt( 2,  nWidth / 2,  nHeight / 2, 0.0, fTextureXMax, fTextureYMax );
        m_pVertexBuffer2->SetAt( 3,  nWidth / 2, -nHeight / 2, 0.0, fTextureXMax, 0.0 );

        // Find zoom factor for full screen window display.
        float fXDirZoom = FullScreenRegion.right / (float)nWidth;
        float fYDirZoom = FullScreenRegion.bottom / (float)nHeight;
        // Get minimum zoom, that can be used for displaying full screen Drawing.
        m_fZoom2 = min( fXDirZoom, fYDirZoom );
    }

    // Get previous window state.
    ::GetWindowRect( m_hParent, &m_stPreviousWindowRegion );

    // Hold previous projection matrix.
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    // Setup projection matrix for full screen display.
    int nWidth = (float)FullScreenRegion.right / 2;
    int nHeight = (float)FullScreenRegion.bottom/ 2;
    glOrtho( -nWidth, nWidth, -nHeight, nHeight, 0.0, 2.0 );

    // take old viewport and set new one.
    glGetIntegerv( GL_VIEWPORT, m_nOldViewPortArray );
    glViewport( 0, 0, FullScreenRegion.right, FullScreenRegion.bottom );

    m_nOldWindowStyle = ::GetWindowLong( m_hParent, GWL_STYLE );

    // int nRemovedStyle = m_nOldWindowStyle 
    // Here change window style to overlapped.
    ::SetWindowLong( m_hParent,GWL_STYLE, WS_OVERLAPPED );

    // Here change width and height of current windoW to Full size.
    ::SetWindowPos( m_hParent, 0, 0, 0, FullScreenRegion.right, FullScreenRegion.bottom, SWP_SHOWWINDOW );



    // Start Timer with 20 ms delay.
    ::SetTimer( m_hParent, TIMER_SLIDE_SHOW, 20, 0 );

    // After 1 second changes the image.
    m_SlideShowTimer.SetMaxElapseTime( ONE_SLIDE_DELAY );

    // Hide mouse cursor
    ::ShowCursor( false );
}

void SlideShow::Stop()
{
    // Here delete all resources.
    delete m_pTexture1;
    delete m_pTexture2;
    delete m_pVertexBuffer1;
    delete m_pVertexBuffer2;

    // Make current displayed image as previous image.
//    m_pFileExplorer->GetPreviousFileName();
    m_pFileExplorer->SetRepeatState( false );

    ::SetWindowLong( m_hParent, GWL_STYLE, m_nOldWindowStyle );

    // Change window size to previous state.
    ::SetWindowPos( m_hParent, 0, m_stPreviousWindowRegion.left, m_stPreviousWindowRegion.top,
                    m_stPreviousWindowRegion.right, m_stPreviousWindowRegion.bottom, SWP_SHOWWINDOW );
    ::KillTimer( m_hParent, TIMER_SLIDE_SHOW );

    // Show mouse cursor.
    ::ShowCursor( true );

    // Reset old viewport.
    glViewport( m_nOldViewPortArray[0], m_nOldViewPortArray[1],
                m_nOldViewPortArray[2], m_nOldViewPortArray[3] );

    // Reset old projection.
    glMatrixMode( GL_PROJECTION );
    glPopMatrix();
}

void SlideShow::Display()
{
    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    // Here clear the background with black.
    glClear( GL_COLOR_BUFFER_BIT );

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();

    // Apply First texture transparency.
    int nRemTime = m_SlideShowTimer.GetRemainingTime();

    if( 0 == nRemTime )
    {
        // Here loop completed, need to swap the textures and create a new texture with 
        // next image.
        CreateNextImage();
        m_SlideShowTimer.SetMaxElapseTime( ONE_SLIDE_DELAY );
        nRemTime = ONE_SLIDE_DELAY;
    }

    // last 100 clocks are used for interpolation.
    float fColorFactor = nRemTime / 100.0;
    fColorFactor = min( 1.0, fColorFactor );

    // glColor3f is used to make small amount of texture display.
    // This color factor is multiplied with texel color and get a shading effect.
    glColor4f( fColorFactor, fColorFactor, fColorFactor, fColorFactor );
    m_pTexture1->Enable();
    // Apply zoom1.
    glScalef( m_fZoom1, m_fZoom1, 0.0 );
    m_pVertexBuffer1->DrawVertexBuffer( GL_QUADS );
    glPopMatrix();

    if( nRemTime < 100 )
    {
        glPushMatrix();
        // When transparent display of second texture is required.
        float fTex2Color = 1.0 - fColorFactor;
        glColor4f( fTex2Color, fTex2Color, fTex2Color, fTex2Color );
        m_pTexture2->Enable();
        // Apply Zoom 2.
        glScalef( m_fZoom2, m_fZoom2, 0.0 );
        m_pVertexBuffer2->DrawVertexBuffer( GL_QUADS );
        glPopMatrix();
    }

    // After one display decrease Slideshow timer
    m_SlideShowTimer.ElapseTime();
    glDisable( GL_BLEND );
}

// This function is called when display of two textures are completed, and now
// need require a new texture for interpolation.
// This function create a new texture with NextFile() of FileExplorer.
// And Swaps texture 2 to Texture 1.
void SlideShow::CreateNextImage()
{
    delete m_pTexture1;
    m_pTexture1 = 0;
    delete m_pVertexBuffer1;
    m_pVertexBuffer1 = 0;


    // Copy texture2 parameters to texture 1.
    m_pTexture1 = m_pTexture2;
    m_pVertexBuffer1 = m_pVertexBuffer2;
    m_fZoom1 = m_fZoom2;

    m_pTexture2 = 0;
    m_pVertexBuffer2 = 0;
    // Create new texture with next image.
    {
        RECT FullScreenRegion;
        ::GetWindowRect( ::GetDesktopWindow(), &FullScreenRegion );

        m_pVertexBuffer2 = new GLVertexBuffer;
        m_pTexture2 = new GLTexture;
        // Creating second texture with next file.
        CString csNextFile = m_pFileExplorer->GetNextFileName();
        BMPLoader Loader;
        int nWidth, nHeight;
        BYTE* pbyData = 0;
        Loader.LoadBMP( csNextFile, nWidth, nHeight, pbyData );
        m_pTexture2->Create( nWidth, nHeight, pbyData );
        delete[] pbyData;

        float fTextureXMax = 1.0;
        float fTextureYMax = 1.0;
        if( !GLExtension::GetInstance().m_bNonPowerOfTwo )
        {
            fTextureXMax = (float)nWidth / PicZoomUtil::GetNearestPowerOf2( nWidth );
            fTextureYMax = (float)nHeight / PicZoomUtil::GetNearestPowerOf2( nHeight );
        }

        // Create vertex buffer for full region window display.
        m_pVertexBuffer2->Create( 4 );
        m_pVertexBuffer2->SetAt( 0, -nWidth / 2, -nHeight / 2, 0.0, 0.0,          0.0 );
        m_pVertexBuffer2->SetAt( 1, -nWidth / 2,  nHeight / 2, 0.0, 0.0,          fTextureYMax );
        m_pVertexBuffer2->SetAt( 2,  nWidth / 2,  nHeight / 2, 0.0, fTextureXMax, fTextureYMax );
        m_pVertexBuffer2->SetAt( 3,  nWidth / 2, -nHeight / 2, 0.0, fTextureXMax, 0.0 );

        // Find zoom factor for full screen window display.
        float fXDirZoom = FullScreenRegion.right / (float)nWidth;
        float fYDirZoom = FullScreenRegion.bottom / (float)nHeight;
        // Get minimum zoom, that can be used for displaying full screen Drawing.
        m_fZoom2 = min( fXDirZoom, fYDirZoom );
    }
}