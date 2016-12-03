#include "StdAfx.h"
#include "GLExtension.h"
#include "Gl\gl.h"

GLExtension::GLExtension(void)
{
    const char* pExensions = (const char* )glGetString( GL_EXTENSIONS );
    m_bNonPowerOfTwo = strstr( pExensions, "GL_ARB_texture_non_power_of_two" );
}

GLExtension::~GLExtension(void)
{
}

GLExtension& GLExtension::GetInstance()
{
    static GLExtension localInstance;
    return localInstance;
}
