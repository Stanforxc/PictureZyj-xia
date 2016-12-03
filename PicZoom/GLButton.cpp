#include "StdAfx.h"
#include "resource.h"
#include "GLButton.h"
#include "GLVertexBuffer.h"
#include "GLTexture.h"
#include "GLExtension.h"
#include "PicZoomUtil.h"
#include "CoordConverter.h"
#include "BMPLoader.h"
#include "CursorManager.h"
#include "Consts.h"


GLButton::GLButton(HWND hParentWindow_i ):
GLWindowBase( hParentWindow_i )
{
    m_bVisible = false;
    m_hParentWindow = hParentWindow_i;
    m_pVertexBuffer = 0;
    m_bACtionStart = false;
    m_fTransparency = 0.0;
}

GLButton::~GLButton(void)
{
}

void GLButton::SetRegion( const int nLeft_i, const int nTop_i,
                              const int nWidth_i, const int nHeight_i )
{
    m_stRegionRect.left = nLeft_i;
    m_stRegionRect.top = nTop_i;
    m_stRegionRect.right = nLeft_i + nWidth_i;
    m_stRegionRect.bottom = nTop_i + nHeight_i;
    if( 0 != m_pVertexBuffer )
    {
        // If vertex buffer is created, then update vertex buffer with new postion.
        UpdateVertexBuffer();
    }
}

void GLButton::SetImage( const int nResourceID_i )
{
    m_nResourceID = nResourceID_i;
}

void GLButton::SetTransparency( const float fTransparency_i )
{
    m_fTransparency = fTransparency_i;
}

// This function is called once, after starting the application.
// All resources required for displaying the button are created in this function,
// The texture and vertex buffer for rendering are setup in this function.
bool GLButton::SetupButton()
{
    // Create Vertex buffer.
    m_pVertexBuffer = new GLVertexBuffer;
    if( 0 == m_pVertexBuffer )
    {
        return false;
    }
    // Create Vertex buffer for rendering Quad image.
    m_pVertexBuffer->CreateQuadVertexBuffer();

    UpdateVertexBuffer();

    // Setup Texture from Bitmap resource ID.
    m_pTexture = new GLTexture;
    int nWidth = 0;
    int nHeight = 0;
    BYTE* pbyARGBImage = 0;
    BMPLoader LoadImage;
    // Load Alpha enabled texture.
    LoadImage.LoadBMP( m_nResourceID, nWidth, nHeight, pbyARGBImage, true );
    // Create RGBA format texture.
    m_pTexture->Create( nWidth, nHeight, pbyARGBImage, GL_RGBA, GL_RGBA8 );
    return (GL_NO_ERROR == glGetError());
}

bool GLButton::Draw()
{
    if( !m_bVisible && 0.0 == m_fTransparency)
    {
        // When visible is false, and transparency is set to 0.0, then nothing to draw.
        return true;
    }
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    // This function render the image with alpha blending to display only the alpha pixels.
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glColor4f( m_fTransparency, m_fTransparency, m_fTransparency, m_fTransparency );

    m_pTexture->Enable();

    if( m_bMouseOver )
    {
        // After drawing, pixelstore biasing is changed.
        //glColor4f( 1.0, 1.0, 1.0, 1.0 );
        glColor4f( m_fTransparency, m_fTransparency, m_fTransparency, m_fTransparency );
    }
    else
    {
        // After drawing, pixelstore biasing is changed.
        //glColor4f( 0.75, 0.75, 0.75, 1.0 );
        glColor4f( 0.75 * m_fTransparency, 0.75 * m_fTransparency, 0.75 * m_fTransparency, 0.75 * m_fTransparency );
    }
    m_pVertexBuffer->DrawVertexBuffer( GL_QUADS );
    m_pTexture->Disable();

    glColor4f( 1.0f,1.0f,1.0f,1.0f);
    glDisable( GL_BLEND );
    return true;
}

// Mouse move returns true if the Mouseover is true, else return false
bool GLButton::OnMouseMove( const int nX_i, const int nY_i, const int nFlags_i )
{
    if( IsWithinRegion( nX_i, nY_i ))
    {
        if( !m_bMouseOver )
        {
            m_bMouseOver = true;
            ::PostMessage( m_hParentWindow, WM_PAINT, 0, 0 );
        }
        //PostMessage( m_hParent, WM_CHANGE_CURSOR_MESSAGE, CURSOR_HAND_TYPE, 0 ); // 0 for Hand cursor.
        (CursorManager::GetInstance())->SetActiveCursor( CURSOR_HAND_TYPE );
        return true;
    }
    else
    {
        if( m_bMouseOver )
        {
            m_bMouseOver = false;
            //PostMessage( m_hParent, WM_CHANGE_CURSOR_MESSAGE, CURSOR_NORMAL_TYPE, 0 ); // 0 for Hand cursor.
            (CursorManager::GetInstance())->SetActiveCursor( CURSOR_NORMAL_TYPE );
            ::PostMessage( m_hParentWindow, WM_PAINT, 0, 0 );
        }
        return false;
    }
}

bool GLButton::OnLButtonDown( const int nX_i, const int nY_i, const int nFlags_i )
{
    if( MK_LBUTTON == ( MK_LBUTTON & nFlags_i ) && IsWithinRegion( nX_i, nY_i ))
    {
        m_bACtionStart = true;
        ::PostMessage( m_hParentWindow, m_nMsgToParent, 0, 0 );
        return true;
    }
    return false;
}


bool GLButton::OnLButtonUp( const int nX_i, const int nY_i, const int nFlags_i )
{
    if( m_bACtionStart )
    {
        m_bACtionStart = false;
        ::PostMessage( m_hParentWindow, m_nMsgToParent, 1, 0 );
        return true;
    }
    return false;
}
void GLButton::SetLButtonMessage( const int nMessageToParent_i )
{
    m_nMsgToParent = nMessageToParent_i;
}

void GLButton::UpdateVertexBuffer()
{
    // Setup 4 coordinate of Vertex buffer.
    // Need to convert Window coordinate to opengl coordinate.
    POINT ptLeftBottom = { m_stRegionRect.left, m_stRegionRect.bottom };
    POINT ptLeftTop = { m_stRegionRect.left, m_stRegionRect.top };
    POINT ptRightTop = { m_stRegionRect.right, m_stRegionRect.top };
    POINT ptRightBottom = { m_stRegionRect.right, m_stRegionRect.bottom };


    // CoordConvert::WindowToGL will convert the window coordinate to opengl coordinate.
    (CoordConverter::GetInstance()).WindowToGL( ptLeftBottom );
    (CoordConverter::GetInstance()).WindowToGL( ptLeftTop );
    (CoordConverter::GetInstance()).WindowToGL( ptRightTop );
    (CoordConverter::GetInstance()).WindowToGL( ptRightBottom );

    float fTextureXMax = 1.0;
    float fTextureYMax = 1.0;
    if( !GLExtension::GetInstance().m_bNonPowerOfTwo )
    {
        fTextureXMax = (float)m_stRegionRect.Width() / PicZoomUtil::GetNearestPowerOf2( m_stRegionRect.Width() );
        fTextureYMax = (float)m_stRegionRect.Height() / PicZoomUtil::GetNearestPowerOf2( m_stRegionRect.Height() ) ;
    }

    // Set Opengl coordinates to Vertex buffer.
    m_pVertexBuffer->SetAt( 0, ptLeftTop.x      , ptLeftTop.y       , 0.0, 0.0,          fTextureYMax );
    m_pVertexBuffer->SetAt( 1, ptLeftBottom.x   , ptLeftBottom.y    , 0.0, 0.0,          0.0 );
    m_pVertexBuffer->SetAt( 2, ptRightBottom.x  , ptRightBottom.y   , 0.0, fTextureXMax, 0.0 );
    m_pVertexBuffer->SetAt( 3, ptRightTop.x     , ptRightTop.y      , 0.0, fTextureXMax, fTextureYMax );
}

bool GLButton::IsWithinRegion( const int nX_i, const int nY_i)
{
    return( m_stRegionRect.left <= nX_i && nX_i <= m_stRegionRect.right &&
            m_stRegionRect.top <= nY_i  && nY_i <= m_stRegionRect.bottom );
}