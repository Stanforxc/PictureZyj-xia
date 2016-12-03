#include "StdAfx.h"
#include "CloseButton.h"
#include "Consts.h"
#include "resource.h"
#include "math.h"
#include "GL\gl.h"


CloseButton::CloseButton(HWND hParent_i):
GLButton( hParent_i )
{
    SetName( L"CloseButton" );
}

CloseButton::~CloseButton(void)
{
}

void CloseButton::SetRegion( const int nWidth_i, const int nHeight_i )
{
    GLWindowBase::SetRegion( nWidth_i, nHeight_i );
    m_nScreenWidth = nWidth_i;
    m_nScreenHeight = nHeight_i;
    // Set up circle center as right top corner.
    m_CloseBoundary.SetPosition( m_nScreenWidth / 2, m_nScreenHeight / 2 );
    GLButton::SetRegion( m_nScreenWidth - 50, 0, 50,50 );
}

bool CloseButton::SetupButton()
{
    // Set properties of Outer circle with transparency.
    m_CloseBoundary.SetAngle( 180, 270 );
    m_CloseBoundary.SetRadius( 43 );
    m_CloseBoundary.SetLineWidth( 3 );
    m_CloseBoundary.SetTransparency( 0.5f );
    m_CloseBoundary.SetColor( 0.4, 0.4, 0.4 );

    // Set base class members to display close button.
    SetImage( IDB_CLOSE_BUTTON );
    SetVisibility( true );
    SetLButtonMessage( WM_CLOSE );
    GLButton::SetRegion( m_nScreenWidth - 50, 0, 50,50 );
    SetTransparency( 1.0f );

    return GLButton::SetupButton();
}

bool CloseButton::Draw()
{
    GLButton::Draw();
    m_CloseBoundary.Draw();
    return true;
}
bool CloseButton::IsWithinRegion( const int nX_i, const int nY_i )
{
    if( GLButton::IsWithinRegion( nX_i, nY_i ))
    {
        // Here check the bottom left corner of circle.
        // If mouse move is not within the semi-circle, then return false.
        int nXDiff = nX_i - GLWindowBase::m_nWidth;
        int nYDiff = nY_i;
        int nRadius = sqrt( (float)nXDiff * nXDiff + (float)nYDiff * nYDiff );
        if( nRadius < 45 )
        {
            return true;
        }
    }
    return false;
}