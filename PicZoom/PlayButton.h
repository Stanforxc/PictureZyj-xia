#pragma once
#include "GLButton.h"

class GLCircle;

const int OUTER_CIRCLES = 2;
/*
This class is derived class of GLButton  PlayButton is circular shaped button, therefore
mouse message handling is specially handled in this class. IsWithinRegion() is overridden in
this class to create a button mouse over effect for circular region of PlayButton.
For smooth edge, the outer region of play button is drawn with 2 GLCircle objects.
*/
class PlayButton :
    public GLButton
{
public:
    PlayButton(HWND hWndParent);
    ~PlayButton(void);
    bool SetupButton();
    void SetRegion( const int nWidth_i, const int nHeight_i );
    bool Draw();
    bool IsWithinRegion( const int nX_i, const int nY_i );

private:
    GLCircle*      m_BoundaryCircles[OUTER_CIRCLES];
    int            m_nWidth;
    int            m_nHeight;
};
