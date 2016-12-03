#include "StdAfx.h"
#include "ZoomText.h"
#include "FontEngine.h"
#include "GLExtension.h"
#include "PicZoomUtil.h"
#include "BMPLoader.h"
#include "resource.h"
#include "GL\gl.h"
#include "math.h"
#include "Consts.h"

const int HEIGHT = 24;
ZoomText::ZoomText(FontEngine* pFontEngine_i, HWND hWnd_i)
{
    m_pFontEngine = pFontEngine_i;
    m_hWnd = hWnd_i;
    m_nTimer = 0;
    m_fZoomValue = 0;

    // Creating Rounded rect.
    CreateRoundedRect();

    // Create Vertex buffer for rendering Texture.
    m_VertexBuffer.CreateQuadVertexBuffer();

    float fTextureXMax = 1.0;
    float fTextureYMax = 1.0;
    if( !GLExtension::GetInstance().m_bNonPowerOfTwo )
    {
        fTextureXMax = (float)m_nTextureWidth / PicZoomUtil::GetNearestPowerOf2( m_nTextureWidth );
        fTextureYMax = (float)m_nTextureHeight / PicZoomUtil::GetNearestPowerOf2( m_nTextureHeight );
    }

    m_VertexBuffer.SetAt( 0, -100, 12, 0.0f, 0.0f,1.0f); // Left Top  corner
    m_VertexBuffer.SetAt( 1, -100,-12, 0.0f, 0.0f,0.0f), // Left Bottom
    m_VertexBuffer.SetAt( 2, 100, -12, 0.0f, 1.0f,0.0f); // Right bottom
    m_VertexBuffer.SetAt( 3, 100, 12,  0.0f, 1.0f,1.0f); // Right 
}

ZoomText::~ZoomText(void)
{
    m_pFontEngine = 0;
}

bool ZoomText::RedrawNeed()
{
    return (bool)m_nTimer;
}
void ZoomText::SetZoomValue( const float fZoomValue_i )
{
    if( m_fZoomValue != fZoomValue_i )
    {
        if( 0 == m_nTimer )
        {
            SetTimer( m_hWnd, TIMER_ZOOM_TEXT, 10, 0 );
        }
        m_nTimer = 50;
        m_fZoomValue = fZoomValue_i;
        
    }
}
void ZoomText::Draw()
{
    if( 0 == m_nTimer )
    {
        // If timer is reset, Zoomtext is not shown.
        return;
    }
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    TCHAR tcBuffer[100];
    swprintf( tcBuffer, L"%d %%", (int)(m_fZoomValue * 100) );

    // +1 is added to get the background width is higher than text width.
    int nLength = m_pFontEngine->GetStringLength( tcBuffer ) + 6;
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glColor4f( 1.0, 1.0, 1.0, m_nTimer / 50.0 );
    m_RoundedRect.Enable();

    float fTextureXMax = 1.0;
    float fTextureYMax = 1.0;
    if( !GLExtension::GetInstance().m_bNonPowerOfTwo )
    {
        fTextureXMax = (float)m_nTextureWidth / PicZoomUtil::GetNearestPowerOf2( m_nTextureWidth );
        fTextureYMax = (float)m_nTextureHeight / PicZoomUtil::GetNearestPowerOf2( m_nTextureHeight );
    }

    int nHalfWidth = nLength / 2;
    m_VertexBuffer.SetAt( 0, -nHalfWidth, 12, 0.0f, 0.0f,1.0f); // Left Top  corner
    m_VertexBuffer.SetAt( 1, -nHalfWidth,-12, 0.0f, 0.0f,0.0f), // Left Bottom
    m_VertexBuffer.SetAt( 2, nHalfWidth, -12, 0.0f, 1.0f,0.0f); // Right bottom
    m_VertexBuffer.SetAt( 3, nHalfWidth, 12,  0.0f, 1.0f,1.0f); // Right 

    m_VertexBuffer.DrawVertexBuffer( GL_QUADS );
    m_RoundedRect.Disable();

    glColor4f( 1.0,1.0,1.0, m_nTimer / 50.0 );
    m_pFontEngine->DrawText( 0,0, tcBuffer );
    glColor4f( 1.0,1.0,1.0,1.0 );
    m_nTimer--;
    if( 0 == m_nTimer )
    {
        KillTimer( m_hWnd, TIMER_ZOOM_TEXT );
        PostMessage( m_hWnd, WM_PAINT, 0, 0 );
    }
    glDisable( GL_BLEND );
}

// This function create a Texture, which holds a Rounded rect 
// This rounded rect is used as background of Zoom text.
void ZoomText::CreateRoundedRect()
{
    BMPLoader LoadImg;
    BYTE* pbyData;
    LoadImg.LoadBMP( IDB_ROUND_RECT, m_nTextureWidth, m_nTextureHeight, pbyData, true );

    m_RoundedRect.Create( m_nTextureWidth, m_nTextureHeight, pbyData, GL_RGBA, GL_RGBA8 );
    delete[] pbyData;
    return;
}