#ifndef _CONSTS_H_
#define _CONSTS_H_
#include "stdafx.h"

const int ZOOM_MAX_VALUE = 30;
typedef enum
{
    CURSOR_HAND_TYPE,
    CURSOR_MOVE_TYPE,
    CURSOR_NORMAL_TYPE,
}CURSOR_TYPE_e;

const float ROTATION_ANGLES[4] = 
{ 0.0,// No rotation
  270.0, // First Clockwise 
  180.0, // Second Clockwise
  90.0 // 3rd clockwise 
};


typedef enum
{
    DIALOG_MODE,
    BACKGROUND_MODE,
    SLIDE_SHOW_MODE,
    MINIMIZE_MODE
}DISPLAY_MODE_e;

typedef enum
{
    TIMER_START = 0,
    TIMER_ZOOM,
    TIMER_TRANSLATE,
    TIMER_BOTTOM_WINDOW_VISIBLITY,
    TIMER_ZOOM_TEXT,
    TIMER_SLIDE_SHOW,
    TIMER_MAKE_TO_CENTER,
    TIMER_MAX
}TIMER_IDs_e;

typedef enum
{
    WM_ZOOM_PLUS_MESSAGE  = WM_USER + 1,
    WM_ZOOM_MIN_MESSAGE,
    WM_ZOOM_1TO1_MESSAGE,
    WM_ROTATE_CLOCK_WISE,
    WM_ROTATE_ANTI_CLOCK_WISE,
    WM_NEXT_FILE_OPEN,
    WM_PREVIOUS_FILE_OPEN,
    WM_PLAY_SLIDE_SHOW,
    WM_MAKE_TO_CENTER,
    WM_NEW_FILE_LOAD,
}WM_PICZOOM_MESSAGES_e;

struct GLColor
{
    GLColor()
    {
        m_fGreen = m_fBlue = m_fRed = 0.0;
    }
    GLColor( float fRed, float fGreen, float fBlue )
    {
        m_fRed = fRed;
        m_fGreen = fGreen;
        m_fBlue = fBlue;
    }
    float m_fRed;
    float m_fGreen;
    float m_fBlue;
};


#endif