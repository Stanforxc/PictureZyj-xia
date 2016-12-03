#include "StdAfx.h"
#include "CursorManager.h"

CursorManager* CursorManager::m_pStaticInstance = 0;
bool CursorManager::CreateInstance( HWND hParentWindow_i )
{
    m_pStaticInstance = new CursorManager( hParentWindow_i );
    return ( 0 != m_pStaticInstance );
}
bool CursorManager::DeleteInstance()
{
    delete m_pStaticInstance;
    m_pStaticInstance = 0;
    return true;
}
CursorManager* CursorManager::GetInstance()
{
    return m_pStaticInstance;
}
HCURSOR CursorManager::GetCurrentCursor()
{
    if( m_hUserCursor )
    {
        return m_hUserCursor;
    }
    return m_hCursorToUse;
}
bool CursorManager::CreateCursors()
{
    m_hMoveCursor = LoadCursor( 0, IDC_SIZEALL );
    m_hHandCursor = LoadCursor( 0, IDC_HAND );
    m_hNormalCursor = LoadCursor( 0, IDC_ARROW );
    return true;
}
bool CursorManager::DeleteCursors()
{
    return true;
}
void CursorManager::SetActiveCursor( const CURSOR_TYPE_e eCursorType_i )
{
    HCURSOR hCursorToUse = 0;
    switch( eCursorType_i )
    {
        case  CURSOR_HAND_TYPE:
        {
            hCursorToUse = m_hHandCursor;
            break;
        }
        case  CURSOR_MOVE_TYPE:
        {
            hCursorToUse = m_hMoveCursor;
            break;
        }
        case  CURSOR_NORMAL_TYPE:
        {
            hCursorToUse = 0;
            break;
        }
        default:
        {
            break;
        }
    }
    m_hUserCursor = hCursorToUse;
}

HCURSOR CursorManager::GetCursor( const CURSOR_TYPE_e eCursorType_i )
{
    HCURSOR hCursorToReturn = 0;
    switch( eCursorType_i )
    {
        case  CURSOR_HAND_TYPE:
        {
            hCursorToReturn = m_hHandCursor;
            break;
        }
        case  CURSOR_MOVE_TYPE:
        {
            hCursorToReturn = m_hMoveCursor;
            break;
        }
        case  CURSOR_NORMAL_TYPE:
        {
            hCursorToReturn = m_hNormalCursor;
            break;
        }
        default:
        {
            break;
        }
    }
    return hCursorToReturn;
}

CursorManager::CursorManager( HWND hParentWindow_i )
{
    m_hParentWindow = hParentWindow_i;
    CreateCursors();
    m_hUserCursor = 0;
    m_hCursorToUse = m_hNormalCursor;
}

CursorManager::~CursorManager(void)
{
    DeleteCursors();
}

