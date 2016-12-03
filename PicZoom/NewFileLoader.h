#pragma once

/*
This class Handles loading of new image.
When user clicks context menu, then new (jpg,or bmp )file should be loaded in 
old process. When an instance of PicZoom is running, the new instance will set an
event and write the name of file in a shared memory, and that memory will be used
to get the name of new file name.
*/
class NewFileLoader
{
public:
    NewFileLoader( HWND hparent );
    ~NewFileLoader(void);

    static UINT ObserveThread(LPVOID pParam);

private:

    bool m_bExit;
    HWND m_hParentWindow;
};
