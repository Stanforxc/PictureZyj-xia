#pragma once

/*
Base class of all opengl windows created in PicZoom.
*/
class GLWindowBase
{
public:

    GLWindowBase(HWND hParent_i);
    virtual ~GLWindowBase(void);
    virtual bool OnMouseMove( const int nX_i, const int nY_i, const int nFlags_i );
    virtual bool OnLButtonDown( const int nX_i, const int nY_i, const int nFlags_i );
    virtual bool OnLButtonUp( const int nX_i, const int nY_i, const int nFlags_i );
    virtual bool OnMouseWheel( const int nX_I, const int nY_i, const UINT nFlags, const short zDelta );
    virtual bool Draw();
    virtual void SetRegion( const int nWidth_i, const int nHeight_i );
    virtual void SetVisibility( const bool bVisible_i );
    virtual void SetName( const CString csWindowName_i );
    virtual CString GetName();
    virtual bool SetupWindow();

protected:

    HWND m_hParentWindow;
    int m_nWidth;
    int m_nHeight;
    bool m_bVisible;
    CString m_csWindowName;
};
