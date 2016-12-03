#include "Consts.h"
#pragma once

/*
This class create different cursors, and any other class can use this class 
for changing the cursor.
This class is a singleton, and any class can get the instance with GetInstance()
*/
class CursorManager
{
public:

    static bool CreateInstance( HWND hParentWindow_i );
    static bool DeleteInstance();
    static CursorManager* GetInstance();
    HCURSOR GetCurrentCursor();
    HCURSOR GetCursor( const CURSOR_TYPE_e eCursorType_i );
    void SetActiveCursor( const CURSOR_TYPE_e eCursorType_i );

private:

    ~CursorManager(void);
    CursorManager( HWND hParentWindow_i );
    bool CreateCursors();
    bool DeleteCursors();

private:

    HCURSOR m_hMoveCursor;
    HCURSOR m_hHandCursor;
    HCURSOR m_hNormalCursor;
    HCURSOR m_hCursorToUse;
    HCURSOR m_hUserCursor;
    HWND    m_hParentWindow;
    static CursorManager* m_pStaticInstance;
};
