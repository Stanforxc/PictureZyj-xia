#include "GLButton.h"
#include "GLCircle.h"

#pragma once

/*
 This class is responsible for draw and message handling of close button.
 Mouse messages are routed to this class.
 When Mouse clicked in this region,  this class send WM_QUIT message to PicZoomDlg.
*/
class CloseButton: public GLButton
{
public:
    CloseButton(HWND hParent_i);
    ~CloseButton(void);
    void SetRegion( const int nWidth_i, const int nHeight_i );
    bool IsWithinRegion( const int nX_i, const int nY_i );
    bool SetupButton();
    bool Draw();

private:

    HWND m_hParent;
    GLCircle      m_CloseBoundary;
    int m_nScreenWidth;
    int m_nScreenHeight;
};
