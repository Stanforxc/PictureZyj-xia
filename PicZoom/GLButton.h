#include "GLWindowBase.h"
#pragma once

class GLVertexBuffer;
class GLTexture;
/*
This class handles all operations related to a Button.
The drawing and mouse message handling is handled in this class.
The resource ID of bitmap is provided to this class, and ID of Message
to send to parentWindow( PicZoomDlg) is also provide to this class.
Whenever user press the button, this class will send message to PicZoomDlg.
*/
class GLButton : public GLWindowBase
{
public:

    GLButton( HWND hParentWindow_i );
    virtual ~GLButton();
    virtual void SetRegion( const int nLeft_i, const int nTop_i,
                    const int nWidth_i, const int nHeight_i );
    virtual void SetImage( const int nResourceID_i );
    virtual bool SetupButton();
    virtual void SetLButtonMessage( const int nMessageToParent_i );
    virtual bool OnMouseMove( const int nX_i, const int nY_i, const int nFlags_i );
    virtual bool OnLButtonDown( const int nX_i, const int nY_i, const int nFlags_i );
    virtual bool OnLButtonUp( const int nX_i, const int nY_i, const int nFlags_i );
    virtual bool IsWithinRegion( const int nX_i, const int nY_i );
    void SetTransparency( const float fTransparency_i );
    virtual bool Draw();

protected:

    void UpdateVertexBuffer();

protected:

    CRect m_stRegionRect;
    int m_nMsgToParent;
    bool m_bMouseOver;
    int  m_nTextureWidth;
    int  m_ntextureHeight;
    GLVertexBuffer* m_pVertexBuffer;
    GLTexture*      m_pTexture;
    int  m_nResourceID;
    bool m_bACtionStart;
    float m_fTransparency;
    int m_nWidth;
    int m_nHeight;
};
