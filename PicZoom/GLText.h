#include "Timer.h"

#pragma once

class FontEngine;

/*
This class simply handles drawing of Text. Show and hide are very smooth
with this class. Subash forced me to create a class. Since he felt 
text display at bottom of window was not smooth, now its okay.
*/
class GLText
{
public:
    GLText( HWND hParentWnd, FontEngine* pFontEngine_i, const int nTimerID_i );
    ~GLText();
    void SetText( const CString csNewText_i );
    void Draw( const int nX_i, const int nY_i );
    bool IsDrawPending();

    FontEngine* m_pFontEngine;
    int         m_nTimer;
    HWND        m_hParentWnd;
    CString     m_csDisplayString;
    CString     m_csDisplayStringNew;
    Timer       m_StringTimerShow;
    Timer       m_StringTimerHide;
};
