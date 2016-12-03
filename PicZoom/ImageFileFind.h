#pragma once
#include "afx.h"

/*
A CFileFind derived class which will find image files only.
*/

class ImageFileFind :
    public CFileFind
{
public:
    ImageFileFind(void);
    ~ImageFileFind(void);

    // Derived functions to find the imagefile.
    BOOL FindNextFile();

private:
    bool m_bReachedEnd;
};
