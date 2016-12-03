#pragma once

/*
This class handles rendering of all texts in PicZoom.
Creating a font display list, and draw all text.
All text drawing is handled with this class.
This class hold width and height of all characters to draw string in
correct alignment and position.
*/
class FontEngine
{
public:
    FontEngine(void);
    ~FontEngine(void);
    bool Create( HDC hDeviceContext_i );
    void Destroy();
    bool DrawText( const int nX_i, const int nY_i, LPCTSTR ptcString );
    int GetStringLength( LPCTSTR ptcString );

private:

    int m_nCharcX[256]; // Width of Characters.
    int m_nCharcY[256]; // Height of characters.
};
