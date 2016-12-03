#include "StdAfx.h"
#include "GLWindowBase.h"

GLWindowBase::GLWindowBase( HWND hParentWindow_i )
{
    m_hParentWindow = hParentWindow_i;
    m_bVisible = false;
    m_nWidth = 0;
    m_nHeight = 0;
}

GLWindowBase::~GLWindowBase(void)
{
}

bool GLWindowBase::OnMouseMove( const int nX_i, const int nY_i, const int nFlags_i )
{
    return false;
}
bool GLWindowBase::OnLButtonDown( const int nX_i, const int nY_i, const int nFlags_i )
{
    return false;
}
bool GLWindowBase::OnLButtonUp( const int nX_i, const int nY_i, const int nFlags_i )
{
    return false;
}
bool GLWindowBase::OnMouseWheel( const int nX_I, const int nY_i, const UINT nFlags, const short zDelta )
{
    return false;
}
bool GLWindowBase::Draw()
{
    return false;
}

void GLWindowBase::SetRegion( const int nWidth_i, const int nHeight_i )
{
    m_nWidth = nWidth_i;
    m_nHeight = nHeight_i;
}

void GLWindowBase::SetVisibility( const bool bVisible_i )
{
    m_bVisible = bVisible_i;
}

bool GLWindowBase::SetupWindow()
{
    return false;
}

void GLWindowBase::SetName( const CString csWindowName_i )
{
    m_csWindowName = csWindowName_i;
}

CString GLWindowBase::GetName()
{
    return m_csWindowName;
}