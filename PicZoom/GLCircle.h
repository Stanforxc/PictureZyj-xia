#pragma once
#include "Consts.h"
/*
This class is used to draw a circle, or semi circle.
The start and end angles are provided to this class.
The draw action create a blended circle, and this class is used
for creating smooth edges for Play button, and Close button.
*/
class GLCircle
{
public:
    GLCircle(void);
    ~GLCircle(void);
    void SetRadius( const int nRadius_i );
    void SetPosition( const int nX_i, const int nY_i );
    void SetAngle( const int nStartAngle_i, const int nEndAngle_i );
    void SetColor( const float fRed_i, const float fGreen_i, const float fBlue_i );
    void SetTransparency( const float fTransparency_i );
    void SetLineWidth( const int nWidth );
    void Draw();

private:

    int m_nRadius;
    int m_nX;
    int m_nY;
    int m_nStartAngle;
    int m_nEndAngle;
    GLColor m_Color;
    float m_fTransparency;
    int   m_nLineWidth;
};
