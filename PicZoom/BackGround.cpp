#include "StdAfx.h"
#include "BackGround.h"
#include "GLVertexBuffer.h"
#include "GLTexture.h"
#include "ScreenCapture.h"
#include "GLExtension.h"
#include "PicZoomUtil.h"
#include "GL\gl.h"

BackGround::BackGround(HWND hParentWnd):
GLWindowBase( hParentWnd ),
m_pVertexBuffer( 0 ),
m_pImage( 0 ),
m_pScreenCaptureObj( 0 )
{
    SetName( L"BackGround" );
    m_pVertexBuffer = new GLVertexBuffer;
    m_pVertexBuffer->CreateQuadVertexBuffer();
}

BackGround::~BackGround(void)
{
    if( m_pVertexBuffer )
    {
        delete m_pVertexBuffer;
        m_pVertexBuffer= 0;
    }

    if( m_pImage )
    {
        delete m_pImage;
        m_pImage = 0;
    }
}

bool BackGround::SetupWindow()
{
    if( !PrepareBackgroundImage())
    {
        return false;
    }

    // Create vertex buffer.
    m_pVertexBuffer->Create( 4 );

    int nHalfOfScreenHeight = m_nHeight  / 2;
    int nHalfOfScreenWidth = m_nWidth/ 2;
    float fTextureXMax = 1.0;
    float fTextureYMax = 1.0;
    if( !GLExtension::GetInstance().m_bNonPowerOfTwo )
    {
        fTextureXMax = (float)m_nWidth / PicZoomUtil::GetNearestPowerOf2( m_nWidth );
        fTextureYMax = (float)m_nHeight / PicZoomUtil::GetNearestPowerOf2( m_nHeight );
    }
    m_pVertexBuffer->SetAt( 0, -nHalfOfScreenWidth,nHalfOfScreenHeight, 0.0f, 0.0f, fTextureYMax); // Left Top  corner
    m_pVertexBuffer->SetAt( 1, -nHalfOfScreenWidth,-nHalfOfScreenHeight, 0.0f, 0.0f, 0.0f), // Left Bottom
    m_pVertexBuffer->SetAt( 2, nHalfOfScreenWidth, -nHalfOfScreenHeight, 0.0f, fTextureXMax, 0.0f); // Right bottom
    m_pVertexBuffer->SetAt( 3, nHalfOfScreenWidth, nHalfOfScreenHeight,  0.0f, fTextureXMax, fTextureYMax ); // Right 
    return true;
}

void BackGround::SetScreenCaptureObj( ScreenCapture* pScreenCaptureObj_i )
{
    m_pScreenCaptureObj = pScreenCaptureObj_i;
}

bool BackGround::Draw()
{
    if(  !GLWindowBase::m_bVisible )
    {
        return true;
    }
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    // Here draws the background image.
    m_pImage->Enable();
    m_pVertexBuffer->DrawVertexBuffer( GL_POLYGON );
    return true;
}


bool BackGround::PrepareBackgroundImage()
{
    if( 0 == m_pScreenCaptureObj )
    {
        return false;
    }
    // ReCreate image on switching from background to Dialog mode.
    if( m_pImage )
    {
        delete m_pImage;
        m_pImage = 0;
    }
    m_pImage = new GLTexture();
    // Create Texture with Desktop data.
    BYTE* pbyScreenCapuredData = m_pScreenCaptureObj->GetBuffer();
    if( 0 == pbyScreenCapuredData )
    {
        AfxMessageBox( L"Buffer is NULL" );
        return false;
    }

    // Set Pixel Transfer to make semitransparent effect of Desktop.
    glPixelTransferf( GL_RED_SCALE, 0.75 );
    glPixelTransferf( GL_GREEN_SCALE, 0.75 );
    glPixelTransferf( GL_BLUE_SCALE, 0.75 );
    // Create Desktop Texture.
    if( !m_pImage->Create( m_nWidth, m_nHeight, pbyScreenCapuredData ))
    {
        CString csError;
        csError.Format( L"Background Texture Creation failed.\n Width:%d Height %d.", m_nWidth, m_nHeight );
        AfxMessageBox( csError );
        return false;
    }

    // After texture load, reset the scale to old value
    glPixelTransferf( GL_RED_SCALE, 1.0 );
    glPixelTransferf( GL_GREEN_SCALE, 1.0 );
    glPixelTransferf( GL_BLUE_SCALE, 1.0 );
    return true;
}
