#pragma once

/*
This class provide static functions for some functionalities required in PicZoom.
GetNearestPowerOf2 provide nearest power of 2 of an integer. This function is used for 
texture creation when non_power_of_two opengl extension is not available.
*/

class PicZoomUtil
{
public:
    PicZoomUtil(void);
    ~PicZoomUtil(void);
    static int GetNearestPowerOf2( const int nNonPowerOftwo_i );
};
