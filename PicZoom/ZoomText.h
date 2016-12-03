#pragma once

#include "GLTexture.h"
#include "GLVertexBuffer.h"

class FontEngine;
/*
This class display current Zoom factor in center of PicZoom Dialog.
Smooth show and hide is implemented with alpha blending.
*/
class ZoomText
{
public:
    ZoomText( FontEngine* pFontEngine_i, HWND hWnd_i );
    ~ZoomText(void);
    bool RedrawNeed();
    void SetZoomValue( const float fZoomValue_i );
    void Draw();

private:

    void CreateRoundedRect();

private:

    FontEngine* m_pFontEngine;
    HWND        m_hWnd;
    GLTexture   m_RoundedRect;
    GLVertexBuffer m_VertexBuffer;
    int         m_nTimer;
    int         m_nTextureWidth;
    int         m_nTextureHeight;
    float       m_fZoomValue;
};
