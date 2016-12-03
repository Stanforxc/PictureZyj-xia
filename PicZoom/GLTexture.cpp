#include "StdAfx.h"
#include "GLTexture.h"
#include "GLExtension.h"
#include "PicZoomUtil.h"
#include "GL\Glu.h"
#include "math.h"

GLTexture::GLTexture(void)
{
	m_nTextureID = 0;
}

GLTexture::~GLTexture(void)
{
	Delete();
}
bool GLTexture::Create(int nWidth, int nHeight, void *pbyData, int nFormat_i, int nInternalFormat_i)
{
    int nError = glGetError();
	if( 0 != m_nTextureID )
	{
		// if this texture already exists then delete it.
		Delete();
	}
	if( 0 == pbyData )
	{
		return false;
	}
	glEnable( GL_TEXTURE_2D );
	glGenTextures( 1, &m_nTextureID );

	glBindTexture( GL_TEXTURE_2D, m_nTextureID );

	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

    bool bNonPowerTwo  = (GLExtension::GetInstance()).m_bNonPowerOfTwo;;

    if( bNonPowerTwo )
    {
	    glTexImage2D( GL_TEXTURE_2D, 0, nInternalFormat_i, nWidth, nHeight, 0,
		    nFormat_i, GL_UNSIGNED_BYTE, pbyData );
    }
    else
    {
        // if non-power of two is not supported, need to create nearest n^2 texture.
        int nNewWidth = PicZoomUtil::GetNearestPowerOf2( nWidth );
        int nNewHeight = PicZoomUtil::GetNearestPowerOf2( nHeight );

        int nChannelCount = ( GL_RGB8 == nInternalFormat_i ) ? 3 : 4;
        int nSize = nNewWidth * nNewHeight * nChannelCount;
        if( 0 != ( nNewWidth * nChannelCount ) % 4 )
        {
            nSize = nNewHeight * ceil( ( nNewWidth * nChannelCount ) / 4.0f ) * 4.0f;
        }
        BYTE* pbyDataNew = new BYTE[nSize];
        memset( pbyDataNew, 0, nSize );
        // Set black data 
        glTexImage2D( GL_TEXTURE_2D, 0, nInternalFormat_i, nNewWidth, nNewHeight, 0,
                      nFormat_i, GL_UNSIGNED_BYTE, pbyDataNew );
        // Update the required area with input data.
        glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, nWidth, nHeight, nFormat_i,
                         GL_UNSIGNED_BYTE, pbyData );
        delete[] pbyDataNew;
    }
    return ( GL_NO_ERROR == glGetError() );
}

bool GLTexture::GetData( BYTE*& pbyData )
{
    if( m_nTextureID )
    {
        glBindTexture( GL_TEXTURE_2D, m_nTextureID );
        glGetTexImage( GL_TEXTURE_2D, 0,GL_BGR_EXT, GL_UNSIGNED_BYTE, pbyData );
        return true;
    }
    return false;
}
void GLTexture::Delete()
{
	if( 0 != m_nTextureID )
	{
		glDeleteTextures( 1, &m_nTextureID );
		m_nTextureID = 0;
	}
}

void GLTexture::Enable()
{
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, m_nTextureID );
}