#pragma once

// This class takes screen shot of Desktop window.
// GetBuffer provide RGB data of Desktop window.
class ScreenCapture
{
public:
    ScreenCapture(void);
    ~ScreenCapture(void);
    bool TakeScreenShot();
    BYTE* GetBuffer();

private:

    BYTE* m_pBuffer;
};
