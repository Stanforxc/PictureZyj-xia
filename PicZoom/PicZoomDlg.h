// PicZoomDlg.h : header file
//
#include "ScreenCapture.h"
#include "UnProject.h"
#include "GLWindowBase.h"
#include "Timer.h"
#include "Consts.h"
#include "afxole.h"
#include<vector>

#pragma once

class GLSetup;
class GLTexture;
class GLVertexBuffer;
class FontEngine;
class ZoomText;
class CloseButton;
class GLButton;
class BottomWindows;
class FileExplorer;
class SlideShow;
class NewFileLoader;

// CPicZoomDlg Dialog class. This class initialize OpenGL and, create main components of PicZoom.
// All messages received in this class are routed to the GLWindowBase* objects in m_Windows list.
class CPicZoomDlg : public CDialog
{
// Construction
public:
	CPicZoomDlg(TCHAR pFilePath[], CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PICZOOM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:

    void Draw();
    void SwitchModeToDialog();
    void SwitchModeToBackground();
    void ChangeWindowSize( const int nWidth_i, const int nHeight_i );
    void LoadImage( LPCTSTR pImagePath_i, bool bNeedToDesktopCapture_i = true );
    void OnCancel();
    void OnOK();
    void RemoveFromWindowList( CString csWindowName_i );
    GLWindowBase* GetFromWindowList( CString csWindowName_i );
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
    afx_msg void OnClose();
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnTimer(  UINT_PTR nIDEvent );
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg);
    afx_msg BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg LRESULT  OnDropFiles(WPARAM wParam,LPARAM lParam);
    afx_msg LRESULT  OnZoomPlus( WPARAM wParam, LPARAM lParam );
    afx_msg LRESULT  OnZoomMinus( WPARAM wParam, LPARAM lParam );
    afx_msg LRESULT  OnZoomOneToOne( WPARAM wParam, LPARAM lParam );
    afx_msg LRESULT  OnRotateClockWise( WPARAM wParam, LPARAM lParam );
    afx_msg LRESULT  OnRotateAntiClockWise( WPARAM wParam, LPARAM lParam );
    afx_msg LRESULT  OnNextFileOpen( WPARAM wParam, LPARAM lParam );
    afx_msg LRESULT  OnPreviousFileOpen( WPARAM wParam, LPARAM lParam );
    afx_msg LRESULT  OnSlideShow( WPARAM wParam, LPARAM lParam );
    afx_msg LRESULT  OnMakeToCenter( WPARAM wParam, LPARAM lParam );
    afx_msg LRESULT  OnNewFileLoad( WPARAM wParam, LPARAM lParam );
    afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
private:

    GLSetup* m_pGLSetup;
    FontEngine*   m_pFontEngine;
    CDC memDC;
    CBitmap bm;
    CPoint m_MouseMoveStart;
    int m_nImageWidth;
    int m_nImageHeight;
    DISPLAY_MODE_e m_eDisplayMode;
    DISPLAY_MODE_e m_eOldDisplayMode;
    int     viewport[4];
    RECT    m_Desktop;
    int    m_nDisplayWidth;
    int    m_nDisplayHeight;
    CString  m_csFileName;
    CString  m_csFileURL;
    RECT     m_DlgRegion;
    POINT    m_ptDlgMoveStart;
    bool     m_bDlgMoveEnabled;
    bool     m_bInitComplete;
    FileExplorer*  m_pFileExplorer;
    SlideShow*     m_pSlideShow;
    int m_nRotate;
    bool m_bDisplayBottomWindow;
    int  m_nRotateOffSet;
    Timer m_RotateTimer;
    bool  m_bStartupLogoDisplay;
    NewFileLoader* m_pNewFileLoader;
    std::vector<GLWindowBase*> m_Windows;
    ScreenCapture   m_ScreenCapture;
};
