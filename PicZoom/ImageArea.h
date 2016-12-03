#include "GLWindowBase.h"
#include "Timer.h"
#pragma once

class GLTexture;
class GLVertexBuffer;
class FontEngine;
class ZoomText;
class UnProject;
/*
This class is responsible for drawing of a Image. Whenever a new image file is loaded,
ImageArea will handle all operations related to one image. Rotate, Zoom, Translation,
are handled in this class. All mouse messages are received from PicZoomDlg to this class,
and handle necessary messages. Required timers are started for Zoom, Translate.
ImageArea uses UnProject class is used to identify the mouse position is within the image area,
and ZoomText class is used to draw current zoom value.
*/
class ImageArea: public
    GLWindowBase
{
public:
    ImageArea( HWND hParent, FontEngine* pFontEngine_i );
    ~ImageArea();
    void SetImageFilePath( CString csFilePath_i );
    void SetImageResourceID( const int nResourceId_i );
    void SetBackgroundCapture( const bool bNeedToCapture_i )
    {
        m_bDesktopCaptured = bNeedToCapture_i;
    }
    bool SetupWindow();
    bool Draw();
    bool OnMouseWheel( const int nX_I, const int nY_i, const UINT nFlags, const short zDelta );
    void GetImageArea( int& nImageWidth_o, int& nImageHeight_o );
    void GetTranslation( int& nXTranslation_o, int& nYTranslation_o );
    void SetTranslation( const int nXTranslation_i, const int nYTranslation_i );
    bool ZoomPlus( const bool bOn_i );
    bool ZoomMin( const bool bOn_i );
    bool ZoomOneToOne();
    bool MakeToCenter();
    bool RotateClockWise();
    bool RotateAntiClockWise();
    bool IsWithinImageArea( const int nX_i, const int nY_i );
    bool OnLButtonDown( const int nX_i, const int nY_i, const int nFlags_i );
    bool OnLButtonUp( const int nX_i, const int nY_i, const int nFlags_i );
    bool OnMouseMove( const int nX_i, const int nY_i, const int nFlags_i ); 

private:

    void ZoomHandling();
    void TranslateHandling();
    void RotateHandling();
    void PrepareZoomDisplay();


private:

    // Members for background image display.
    GLVertexBuffer* m_pVertexBuffer;
    GLTexture*      m_pImage;
    int             m_nImageWidth;
    int             m_nImageHeight;
    float           m_fZoom;
    float           m_fZoomOffset;
    CString         m_csFilePath;
    int             m_nResourceID;
    Timer           m_ZoomTimer;
    Timer           m_TranslateTimer;
    Timer           m_RotateTimer;
    float           m_fXTranslation;
    float           m_fYTranslation;
    float           m_fXTranslationOffSet;
    float           m_fYTranslationOffSet;
    ZoomText*       m_pZoomText;
    UnProject*      m_pUnProject;
    bool            m_bDesktopCaptured;
    bool            m_bContinousZoom;
    bool            m_bPanEnabled;
    int             m_nRotate;
    POINT           m_MouseMoveStart;
};
