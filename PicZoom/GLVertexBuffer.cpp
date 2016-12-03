#include "StdAfx.h"
#include "GLVertexBuffer.h"


GLVertexBuffer::GLVertexBuffer(void)
{
    m_nRotate = 0;
	m_pVertexBuffer = 0;
	m_nCount = 0;
}

GLVertexBuffer::~GLVertexBuffer(void)
{
	Delete();
}

bool GLVertexBuffer::Create( int nCount_i )
{
	// Delete if already exists.
	Delete();

	m_nCount = nCount_i;
	m_pVertexBuffer = new GLVertex[nCount_i];
	if( 0 == m_pVertexBuffer )
	{
		return false;
	}
	memset( m_pVertexBuffer, 0, sizeof( GLVertex ) * m_nCount );
	return true;

}
void GLVertexBuffer::Delete()
{
	if( m_pVertexBuffer )
	{
		delete[] m_pVertexBuffer;
		m_pVertexBuffer = 0;
	}
}

// Here change vertex information of a particular index.
bool GLVertexBuffer::SetAt( int nIndex_i, float fX_i, float fY_i, float fZ_i, float fTu_i, float fTv_i )
{
	if( nIndex_i >= m_nCount  )
	{
		return false;
	}
	m_pVertexBuffer[nIndex_i].x = fX_i;
	m_pVertexBuffer[nIndex_i].y = fY_i;
	m_pVertexBuffer[nIndex_i].z = fZ_i;
	m_pVertexBuffer[nIndex_i].tu = fTu_i;
	m_pVertexBuffer[nIndex_i].tv = fTv_i;
	return true;
}

void GLVertexBuffer::DrawVertexBuffer( GLenum eDrawPrimitive_i )
{
    
    glInterleavedArrays( GL_T2F_V3F, 0, m_pVertexBuffer );
    glDrawArrays( eDrawPrimitive_i, 0, m_nCount );
}

void GLVertexBuffer::CreateQuadVertexBuffer()
{
    Create( 4 );
    SetAt( 0, -1.0f,1.0f, 0.0f, 0.0f,0.0f); // Left Top  corner
    SetAt( 1, -1.0f,-1.0f, 0.0f, 1.0f,0.0f), // Left Bottom
    SetAt( 2, 1.0f , -1.0f, 0.0f, 1.0f,1.0f); // Right bottom
    SetAt( 3, 1.0f, 1.0f,  0.0f, 0.0f,1.0f); // Right top
}

void GLVertexBuffer::Rotate( int nRotate_i )
{
    m_nRotate = nRotate_i;
    GLVertexBuffer VertexBufferRotate;
    VertexBufferRotate.CreateQuadVertexBuffer();
    for( int nIndex = 0; nIndex < 4; nIndex++ )
    {
        GLVertex& Temp = m_pVertexBuffer[nIndex];
        int nNext = nIndex + 1;
        if( nNext >= 4 )
        {
            nNext = 0;
        }
        GLVertex& TempV = m_pVertexBuffer[nNext];
        VertexBufferRotate.SetAt( nIndex, Temp.x, Temp.y, Temp.z , TempV.tu, TempV.tv );
    }

    {
        for( int nIndex = 0; nIndex < 4; nIndex++ )
        {
            GLVertex& Temp = VertexBufferRotate.Get(nIndex);
            SetAt( nIndex, Temp.x, Temp.y, Temp.z , Temp.tu, Temp.tv );
        }
    }
}