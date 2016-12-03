#include "StdAfx.h"
#include "ScreenCapture.h"
#include "math.h"

ScreenCapture::ScreenCapture(void)
{
    m_pBuffer = 0;
}

ScreenCapture::~ScreenCapture(void)
{
    if( 0 !=  m_pBuffer )
    {
        delete[] m_pBuffer;
        m_pBuffer = 0;
    }
}

// This function takes screen shot of Desktop window,
// Then allocate a buffer, and copies RGB data to that buffer.
bool ScreenCapture::TakeScreenShot()
{
    // Get Desktop window.
    HWND hDesktop = ::GetDesktopWindow();

    // Get temperory Dc for getting data from Desktop.
    CDC dc;
    HDC hdc = ::GetWindowDC(hDesktop);
    dc.Attach(hdc);

    CDC MemoryDC;
    MemoryDC.CreateCompatibleDC(&dc);

    CRect WindowRegion;
    ::GetWindowRect( hDesktop, &WindowRegion);

    CBitmap BmpObj;
    //DesktopRegion.bottom = ;
    CSize sz(WindowRegion.Width(),::GetSystemMetrics( SM_CYFULLSCREEN ) + ::GetSystemMetrics( SM_CYSIZE ));
    BmpObj.CreateCompatibleBitmap(&dc, sz.cx, sz.cy);
    CBitmap * oldbm = MemoryDC.SelectObject(&BmpObj);
    // Get data from Desktop to Bitmap.
    MemoryDC.BitBlt(0, 0, sz.cx, sz.cy, &dc, 0, 0, SRCCOPY);

    // Reset Dc.
    MemoryDC.SelectObject(oldbm);
    if( m_pBuffer )
    {
        delete[] m_pBuffer;
        m_pBuffer = 0;
    }
    int nNumBytes = ceil( sz.cx * 3 / 4.0 ) * 4.0 * sz.cy;
    m_pBuffer = new BYTE[nNumBytes];
    if( 0 == m_pBuffer )
    {
        return false;
    }
    memset( m_pBuffer, 0, nNumBytes );

    // Get RGB buffer from Bitmap.
    BITMAPINFO stBitmapInfo;
    stBitmapInfo.bmiHeader.biClrImportant = 0;
    stBitmapInfo.bmiHeader.biClrUsed = 0;
    stBitmapInfo.bmiHeader.biCompression = 0;
    stBitmapInfo.bmiHeader.biPlanes = 1;
    stBitmapInfo.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
    stBitmapInfo.bmiHeader.biXPelsPerMeter = 0;
    stBitmapInfo.bmiHeader.biYPelsPerMeter = 0;
    stBitmapInfo.bmiHeader.biBitCount = 3 * 8;
    stBitmapInfo.bmiHeader.biSizeImage = sz.cy * ceil(( sz.cx * 3) / 4.0) * 4.0;
    stBitmapInfo.bmiHeader.biHeight = sz.cy;
    stBitmapInfo.bmiHeader.biWidth = sz.cx;
    stBitmapInfo.bmiColors[0].rgbBlue = 0x0;
    stBitmapInfo.bmiColors[0].rgbGreen = 0x0;
    stBitmapInfo.bmiColors[0].rgbRed = 0x0;
    stBitmapInfo.bmiColors[0].rgbReserved = 0x0;

   int nStatus = ::GetDIBits( MemoryDC.m_hDC, (HBITMAP)BmpObj.m_hObject, 0, sz.cy,
        m_pBuffer, &stBitmapInfo,DIB_RGB_COLORS );

    BmpObj.Detach();  // make sure bitmap not deleted with CBitmap object
    return( 0 != nStatus );
}

BYTE* ScreenCapture::GetBuffer()
{
    // Return captured Screen buffer.
    return m_pBuffer;
}
