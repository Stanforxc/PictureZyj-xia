#include "StdAfx.h"
#include "PicZoomUtil.h"
#include "math.h"

PicZoomUtil::PicZoomUtil(void)
{
}

PicZoomUtil::~PicZoomUtil(void)
{
}

int PicZoomUtil::GetNearestPowerOf2( const int nNonPowerOftwo_i )
{
    for( int nIndex = 0; nIndex < 100; nIndex++ )
    {
        if( pow( (float)nIndex, (float)2 ) > nNonPowerOftwo_i )
        {
            return pow( (float)nIndex, (float)2 );
        }
    }
    return 0;
}
