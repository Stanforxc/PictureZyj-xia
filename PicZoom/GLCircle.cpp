#include "StdAfx.h"
#include "GLCircle.h"
#include "math.h"
#include "GL\Gl.h"

const float DEG2RAD = 3.14159/180.0;

GLCircle::GLCircle(void)
{
    m_nLineWidth = 1;
    m_nStartAngle = 0;
    m_nX = 0;
    m_nY = 0;
    m_nEndAngle = 360;
    m_nRadius = 10;
    m_fTransparency = 1.0;
}

GLCircle::~GLCircle(void)
{

}

void GLCircle::SetRadius( const int nRadius_i )
{
    m_nRadius = nRadius_i;
}

void GLCircle::SetPosition( const int nX_i, const int nY_i )
{
    m_nX = nX_i;
    m_nY = nY_i;
}
void GLCircle::SetAngle( const int nStartAngle_i, const int nEndAngle_i )
{
    m_nStartAngle = nStartAngle_i;
    m_nEndAngle = nEndAngle_i;
}
void GLCircle::SetColor( const float fRed_i, const float fGreen_i, const float fBlue_i )
{
    m_Color = GLColor( fRed_i, fGreen_i, fBlue_i );
}
void GLCircle::SetTransparency( const float fTransparency_i )
{
    m_fTransparency = fTransparency_i;
}

void GLCircle::SetLineWidth( const int nWidth_i )
{
    m_nLineWidth = nWidth_i;
}

void GLCircle::Draw()
{
    glMatrixMode( GL_MODELVIEW );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glLineWidth( m_nLineWidth );
    glPushMatrix();
    glTranslatef( m_nX, m_nY, 0 );
    glColor4f( m_Color.m_fRed, m_Color.m_fGreen, m_Color.m_fBlue , m_fTransparency );
    glBegin( GL_LINE_STRIP );
    for (int i=m_nStartAngle; i <= m_nEndAngle; i++)
    {
        float degInRad = i*DEG2RAD;
        glVertex2f(cos(degInRad)*m_nRadius,sin(degInRad)*m_nRadius);
    }
    glEnd();
    glColor3f( 1.0, 1.0, 1.0 );
    glPopMatrix();
    glDisable( GL_LINE_SMOOTH );
    glDisable( GL_POLYGON_SMOOTH );
    glDisable( GL_BLEND );
}
