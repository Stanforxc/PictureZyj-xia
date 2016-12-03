#pragma once

/*
// There is a change in opengl coordinate representation, and the windows coordinate 
// representation. This change is applied only to achieve zoom functionality. When applying
// zoom( glScale), the scale should be equally applied to all coordinates.
// The expected output can only be achieved by making 0,0 as center point.
// We need to convert the window coordinate to opengl based on window region.

(0,0)--------------------------(Width,0)|
|                                       |
|                                       |
|           Normal Coordinate           |
|                                       |
|                                       |
(0,Height)------------------(Width,Heght)


// Opengl coordiante used in this project.
(-Wid/2,Hei/2)---------------(Wid/2,Hei/2)
|                   |                    |
|                   |                    |
|                   |                    |
|-----------------(0,0)------------------|
|                   |                    |
|                   |                    |
|                   |                    |
(-Wid/2,-Hei/2)--------------(Wid/2,-Hei/2)

This class holds current window region, and it convert the 
window coordinate to opengl coordinate. This can be used for vertex buffer
creation with window coordinates.
*/
class CoordConverter
{
public: 

    static CoordConverter& GetInstance()
    {
        return m_MyStatiInstance;
    }

    void SetWindowRegion( const int nWidth_i, const int nHeight_i );
    void WindowToGL( POINT& ptPoint_io );

private:

    CoordConverter(void);
    ~CoordConverter(void);

private:

    int m_nXOffset;
    int m_nYOffset;
    static CoordConverter m_MyStatiInstance;
};
