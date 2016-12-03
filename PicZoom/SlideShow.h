#pragma once
#include "ImageFileFind.h"
#include "Timer.h"

class GLTexture;
class GLVertexBuffer;
class FileExplorer;

/*
This class is responsible for displaying the Slideshow of a folder.
This class creates two textures, and make the combined image with alpha blending.
ImageFileFinder class is used to find the image files.
*/
class SlideShow
{
public:

    SlideShow(HWND hParentWindow_i, FileExplorer* pFileExplorer_i );
    ~SlideShow(void);
    void Display();
    void Start();
    void Stop();

private:

    void CreateNextImage();

private:

    HWND          m_hParent;
    FileExplorer* m_pFileExplorer;
    ImageFileFind m_ImageFileFinder;
    GLTexture*    m_pTexture1;
    GLTexture*    m_pTexture2;
    GLVertexBuffer* m_pVertexBuffer1;
    GLVertexBuffer* m_pVertexBuffer2;
    float           m_fZoom1;
    float           m_fZoom2;
    Timer           m_SlideShowTimer;
    RECT            m_stPreviousWindowRegion;
    int             m_nOldViewPortArray[4];
    int             m_nOldWindowStyle;
};
