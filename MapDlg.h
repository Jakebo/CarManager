//{{AFX_INCLUDES()
#include "webbrowser2.h"
//}}AFX_INCLUDES
#if !defined(AFX_MAPDLG_H__D28C1628_E7D4_452B_BE3F_D1ED2D90ABCA__INCLUDED_)
#define AFX_MAPDLG_H__D28C1628_E7D4_452B_BE3F_D1ED2D90ABCA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MapDlg.h : header file
//

#include "MyTabCtrl.h"
#include "WebPage.h"
#include "TargetOpt.h"

/////////////////////////////////////////////////////////////////////////////
// CMapDlg dialog
class CGpsDlg;
class CLogDlg;

class CMapDlg : public CDialog
{
// Construction
// 默认构造函数已经被删掉
public:
	CMapDlg(CString &purviewCoding, CWnd* pParent = NULL);   // standard constructor

	// 响应地图上的调用
	void GetCarInfo(const int targetId);
	void CreateTargetMarker();
	void CreateTraceMarker(const int id);

// Dialog Data
	//{{AFX_DATA(CMapDlg)
	enum { IDD = IDD_DIALOG1 };
	CWebBrowser2	m_web;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	void CallJSFunc(const CString &funcName, const CStringArray &args);
	void ModuleSetup(CString &purviewCoding); 
	CWebBrowser2 &GetWebBrowser();
	void Resize();

	// Generated message map functions
	//{{AFX_MSG(CMapDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
private:
	CString &purviewCoding; // 保存权限编码
	POINT old; // Store the dialog's width & height
	CMyTabCtrl m_tab;
	CWebPage m_webPage;
	CGpsDlg *gpsDlg;
	CTargetOpt targetOpt;

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPDLG_H__D28C1628_E7D4_452B_BE3F_D1ED2D90ABCA__INCLUDED_)
