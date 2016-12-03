#include "StdAfx.h"
#include "FontEngine.h"
#include "Consts.h"
#include "GL\gl.h"

FontEngine::FontEngine(void)
{
}

FontEngine::~FontEngine(void)
{
    Destroy();
}

bool FontEngine::Create( HDC hDeviceContext_i )
{
    // Creating font
    CFont *m_pFont;
    m_pFont = new CFont();

    VERIFY(m_pFont->CreateFont(
        15, // nHeight
        7, // nWidth
        0, // nEscapement
        0, // nOrientation
        FW_BOLD, // nWeight
        FALSE, // bItalic
        FALSE, // bUnderline
        0, // cStrikeOut
        ANSI_CHARSET, // nCharSet
        OUT_DEFAULT_PRECIS, // nOutPrecision
        CLIP_DEFAULT_PRECIS, // nClipPrecision
        ANTIALIASED_QUALITY, // nQuality
        DEFAULT_PITCH, // nPitchAndFamily
        L"Arial")); // lpszFacename

    HGDIOBJ hOldFont = ::SelectObject(hDeviceContext_i, m_pFont->m_hObject);

    // Matrix for GetGlyphOutline.
    MAT2 m2 = {{0, 1}, {0, 0}, {0, 0}, {0, 1}}; // identity matrix

    // Retrieve Width and height of each characters.
    // This information is used to find out the width of a text.
    GLYPHMETRICS glyphInfo;
    ZeroMemory(&glyphInfo,sizeof(GLYPHMETRICS));

    CString cs;
    BYTE pby[2222];
    memset( m_nCharcX, 0, sizeof(m_nCharcX));
    memset( m_nCharcY, 0, sizeof(m_nCharcY));
    for( int nChar = 0; nChar <= 255; nChar++ )
    {
        int nPrevErr = GetLastError();
        int nerr1 = GetGlyphOutline(
            hDeviceContext_i,
            nChar, // current char
            GGO_METRICS, &glyphInfo, 2222, pby, &m2
            );
        // Work around to make the width of 0 to 9 are same.
        // otherwise position of % marker will change in Zoomtext
        if( isdigit(nChar))
        {
            glyphInfo.gmBlackBoxX = 8;
        }
        m_nCharcX[nChar] = glyphInfo.gmBlackBoxX;
        m_nCharcY[nChar] = glyphInfo.gmBlackBoxY;
    }

    if( !wglUseFontBitmaps( hDeviceContext_i, 0, 256 * 2, 1000 ))
    {
        return false;
    }

    ::SelectObject( hDeviceContext_i, hOldFont );

    delete m_pFont;
    return true;
}
void FontEngine::Destroy()
{
    glDeleteLists( 1000, 256 );
}

bool FontEngine::DrawText( const int nX_i, const int nY_i, LPCTSTR ptcString )
{
    int nLen = GetStringLength( ptcString ) / 2;
    glRasterPos2d( nX_i - nLen, nY_i - 5 );
    glListBase( 1000 );
    glCallLists( wcslen( ptcString ), GL_UNSIGNED_SHORT, ptcString );
    return true;
}

int FontEngine::GetStringLength( LPCTSTR ptcString )
{
    int nLength = 0;
    int nMax = wcslen( ptcString );
    LPCTSTR ptcStringTemp = ptcString;
    for( int n = 0; n < nMax ; n++ )
    {
        // +2 added for gap between two characters.
        nLength += m_nCharcX[*ptcStringTemp++] + 2;
    }
    return nLength;
}