#pragma once

// This class is used to get opengl vertex coordinate from screen coordinate.
// And then identify whether the specified position is within the Image area.
class UnProject
{
public:
    UnProject( float& fZoom_i,
               float& fPanX_i, float& fPanY_i, int& nRotionIndex );
    ~UnProject(void);
    void SetImageArea( const int nLeft, const int nTop_i, 
                       const int nWidth_i, const int nHeight_i );
    bool IsWithinRange( const int nX_i, const int nY_i );

private:
    int m_nLeft;
    int m_nRight;
    int m_nTop;
    int m_nBottom;
    float& m_fPanX;
    float& m_fPanY;
    float& m_fZoom;
    int&   m_nRotateIndex;
};
