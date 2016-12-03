#include "StdAfx.h"
#include "PlayButton.h"
#include "GLCircle.h"
#include "math.h"
#include "resource.h"


PlayButton::PlayButton(HWND hWndParent):
GLButton(hWndParent)
{
    for( int nIndex = 0; nIndex < OUTER_CIRCLES; nIndex++ )
    {
        m_BoundaryCircles[nIndex] = 0;
    }
}

PlayButton::~PlayButton(void)
{
    for( int nIndex = 0; nIndex < OUTER_CIRCLES; nIndex++ )
    {
        if( m_BoundaryCircles[nIndex] )
        {
            delete m_BoundaryCircles[nIndex];
            m_BoundaryCircles[nIndex] = 0;
        }
    }
}

bool PlayButton::SetupButton()
{
    GLButton::SetImage( IDB_PLAY_BUTTON );
    GLButton::SetVisibility( true );
    GLButton::SetLButtonMessage( WM_PLAY_SLIDE_SHOW );
    GLButton::SetRegion( m_nWidth / 2 - 15, m_nHeight - 32, 32, 32 );
    GLButton::SetupButton();

    for( int nIndex = 0; nIndex < OUTER_CIRCLES; nIndex++ )
    {
        if( m_BoundaryCircles[nIndex] )
        {
            delete m_BoundaryCircles[nIndex];
            m_BoundaryCircles[nIndex] = 0;
        }
    }

    // Here Setup outline circles for smooth edge for Playbutton.
    for( int nIndex = 0; nIndex < OUTER_CIRCLES; nIndex++ )
    {
        m_BoundaryCircles[nIndex] = new GLCircle();
        // m_nWidth / 2 - 40, 40, 62, 68
        // Set up circle for outer boundary.
        m_BoundaryCircles[nIndex]->SetPosition( 0, -m_nHeight / 2 + 15 );
        m_BoundaryCircles[nIndex]->SetAngle( 0, 360 );
        m_BoundaryCircles[nIndex]->SetRadius( 13 + nIndex );
        m_BoundaryCircles[nIndex]->SetLineWidth( 2 );
        m_BoundaryCircles[nIndex]->SetTransparency( 0.25 * GLButton::m_fTransparency);
        m_BoundaryCircles[nIndex]->SetColor( 0.5, 0.5, 0.5 );
    }

    return true;
}
void PlayButton::SetRegion( const int nWidth_i, const int nHeight_i )
{
    m_nWidth = nWidth_i;
    m_nHeight = nHeight_i;

    // Here change position of Play button.
    GLButton::SetRegion( m_nWidth / 2 - 15, m_nHeight - 32, 32, 32 );
}
bool PlayButton::Draw()
{
    GLButton::Draw();


    // After drawing texture, drawing outline with alpha to make transparent edge.
    for( int nIndex = 0; nIndex < OUTER_CIRCLES; nIndex++ )
    {
        m_BoundaryCircles[nIndex]->SetTransparency( 0.25 * GLButton::m_fTransparency);
       m_BoundaryCircles[nIndex]->Draw();
    }
    return true;
}

bool PlayButton::IsWithinRegion( const int nX_i, const int nY_i )
{
    if( GLButton::IsWithinRegion( nX_i, nY_i ) )
    {
        // This function checks mouse position is within circle play button.

        // Find radius from center of Play button to current mouse position.
        int nXDiff = nX_i - ( m_nWidth / 2 );
        int nYDiff = nY_i - m_nHeight;
        int nRadius = sqrt( (float)nXDiff * nXDiff + (float)nYDiff * nYDiff );
        if( nRadius < 29 )
        {
            return true;
        }
    }
    return false;
}

