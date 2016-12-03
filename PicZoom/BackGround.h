#pragma once
#include "glwindowbase.h"

class ScreenCapture;
class GLVertexBuffer;
class GLTexture;

/*
This class handles drawing of background image displayed in PicZom.
When full screen is on, the texture for the desktop image is created in this class.
During each draw, this texture is drawn to the screen.
*/
class BackGround :
    public GLWindowBase
{
public:

    BackGround(HWND hParentWnd);
    ~BackGround(void);
    bool Draw();
    bool SetupWindow();
    void SetScreenCaptureObj( ScreenCapture* pScreenCaptureObj_i );

private:

    bool PrepareBackgroundImage();

private:

    // Members required to display the background image.
    GLTexture* m_pImage;
    GLVertexBuffer* m_pVertexBuffer;
    ScreenCapture*  m_pScreenCaptureObj;
};
