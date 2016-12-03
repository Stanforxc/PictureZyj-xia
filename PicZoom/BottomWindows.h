#include <vector>
#include "GLCircle.h"
#include "GLWindowBase.h"
#include "Timer.h"

#pragma once
class GLButton;
class PlayButton;
class FontEngine;
class GLText;

/*
This class holds all windows in the bottom area of PicZoom.
All GLButton objects are created in this class, and messages are routed through 
 this class. PicZoomDlg will create BottomWindows, and then send all messages to 
 this class.
*/
class BottomWindows : public GLWindowBase
{
public:

    BottomWindows(HWND hParentWnd, FontEngine* pFontEngine_i );
    ~BottomWindows(void);
    bool Initialize();
    void SetRegion( const int nWidth_i, const int nHeight_i );
    bool OnMouseMove( const int nX_i, const int nY_i, const int nFlags_i );
    bool OnLButtonDown( const int nX_i, const int nY_i, const int nFlags_i );
    bool OnLButtonUp( const int nX_i, const int nY_i, const int nFlags_i );
    void SetVisibility( const bool bVisible_i );
    void SetMessage( CString csMessage_i );
    bool Draw();

    GLButton* m_pButtonZoomMin;
    GLButton* m_pButtonZoomMax;
    GLButton* m_pButtonZoomOneToOne;
    GLButton* m_pButtonNext;
    GLButton* m_pButtonPlay;
    GLButton* m_pButtonPrevious;
    GLButton* m_pButtonRotateCW;
    GLButton* m_pButtonRotateACW;
    GLButton* m_pButtonMakeCenter;
    std::vector<GLButton*> m_WindowList;
    std::vector<CString> m_WindowFunctionality;
    PlayButton*    m_pPlayButton;
    GLText*        m_pWindowName;
    CString        m_csMessage;
    Timer         m_DrawTimer;
};
