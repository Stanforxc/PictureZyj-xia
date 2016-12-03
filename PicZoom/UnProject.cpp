#include "StdAfx.h"
#include "UnProject.h"
#include "gl\gl.h"
#include "gl\glu.h"
#include "Consts.h"

UnProject::UnProject( float& fZoom_i, float& fPanX_i, float& fPanY_i, int& nRotateIndex_i )
: m_fZoom( fZoom_i )
, m_fPanX( fPanX_i )
, m_fPanY( fPanY_i )
, m_nRotateIndex( nRotateIndex_i )
{
}

UnProject::~UnProject(void)
{
}

void UnProject::SetImageArea( const int nLeft, const int nTop_i, 
                  const int nWidth_i, const int nHeight_i )
{
    m_nLeft = nLeft;
    m_nTop = nTop_i;
    m_nRight = nLeft + nWidth_i;
    m_nBottom = nTop_i + nHeight_i;
}
bool UnProject::IsWithinRange( const int nX_i, const int nY_i )
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();
    glTranslatef( m_fPanX, -m_fPanY, 0.0 );
    glScalef( m_fZoom, m_fZoom, 1.0 );
    float fRotationAngle = ROTATION_ANGLES[m_nRotateIndex];
    glRotatef( fRotationAngle, 0, 0, 1.0 );

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    winX = (float)nX_i;
    winY = (float) viewport[3] - nY_i;
    winZ = 0.0;
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    glPopMatrix();
    return( m_nLeft <= posX && posX <= m_nRight &&
        m_nTop <= posY && posY <= m_nBottom );

}