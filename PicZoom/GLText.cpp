#include "StdAfx.h"
#include "GLText.h"
#include "FontEngine.h"
#include "GL\gl.h"

GLText::GLText( HWND hParentWnd, FontEngine* pFontEngine_i, const int nTimerID_i )
{
    m_hParentWnd = hParentWnd;
    m_pFontEngine = pFontEngine_i;
    m_nTimer = nTimerID_i;
    m_csDisplayString = "";
    m_csDisplayStringNew = "";
}

GLText::~GLText(void)
{
}

void GLText::SetText(const CString csNewText_i)
{
    if( csNewText_i.Compare( m_csDisplayStringNew ))
    {
        //if( !m_bNullString )
        {
            m_csDisplayStringNew = csNewText_i;
        }
        // If Timer is 
        //   bool bTimerNotNeeded = !( m_bNullString && m_csDisplayString.GetLength() &&m_DisplayStringTimer.IsEnabled());
        // Subhash's comment. The transition of string is not smooth.
        // Here window name is changed.
        // Setup a timer for alpha blending of text display.
        m_StringTimerShow.SetMaxElapseTime( 20 );
        if( !m_StringTimerHide.IsEnabled() )
        {
            // If already one hide is started, then need to handle it.
            m_StringTimerHide.SetMaxElapseTime( 20 );
        }

        if( 0 == m_csDisplayString.Compare( L"" ))
        {
            m_StringTimerHide.SetMaxElapseTime( 0 );
        }


        if( 0 == m_csDisplayStringNew.Compare( L"" ))
        {
            m_StringTimerShow.SetMaxElapseTime( 0 );
        }

        ::SetTimer( m_hParentWnd, m_nTimer, 10, 0 );
    }
}

void GLText::Draw(const int nX_i, const int nY_i)
{
    if( m_StringTimerHide.IsEnabled())
    {
        // Hide old string
        int nRemTime = m_StringTimerHide.GetRemainingTime();
        float fColorComponent = ( nRemTime / 20.0 );
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        glColor4f( 1.0, 1.0, 1.0, fColorComponent );
        // Drawing text to screen.
        m_pFontEngine->DrawText( nX_i, nY_i, m_csDisplayString );
        glDisable( GL_BLEND );
        glColor4f( 1.0, 1.0, 1.0, 1.0 );
        m_StringTimerHide.ElapseTime();
    }
    else
    {
        m_csDisplayString = m_csDisplayStringNew;
        // Show New string.
        int nRemTime = 20 - m_StringTimerShow.GetRemainingTime();
        float fColorComponent = ( nRemTime / 20.0 );
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        glColor4f( 1.0, 1.0, 1.0, fColorComponent );
        // Drawing text to screen.
        m_pFontEngine->DrawText( nX_i, nY_i, m_csDisplayString );
        glDisable( GL_BLEND );
        glColor4f( 1.0, 1.0, 1.0, 1.0 );
        m_StringTimerShow.ElapseTime();
    }
}

bool GLText::IsDrawPending()
{
    return ( m_StringTimerShow.IsEnabled() || m_StringTimerHide.IsEnabled() );
}