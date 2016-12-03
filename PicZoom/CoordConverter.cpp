#include "StdAfx.h"
#include "CoordConverter.h"
CoordConverter CoordConverter::m_MyStatiInstance;
CoordConverter::CoordConverter(void)
{
    m_nXOffset = m_nYOffset = 0;
}

CoordConverter::~CoordConverter(void)
{
}

void CoordConverter::SetWindowRegion( const int nWidth_i, const int nHeight_i )
{
    m_nXOffset = - nWidth_i / 2;
    m_nYOffset = -nHeight_i / 2;
}
void CoordConverter::WindowToGL( POINT& ptPoint_io )
{
    ptPoint_io.x += m_nXOffset;
    ptPoint_io.y += m_nYOffset;
    ptPoint_io.y *= -1;
}