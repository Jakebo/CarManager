//{{AFX_INCLUDES()
#include "webbrowser2.h"
//}}AFX_INCLUDES
#if !defined(AFX_TRACEDLG_H__2A1CA561_9259_44B0_81F6_E8FD340D9A90__INCLUDED_)
#define AFX_TRACEDLG_H__2A1CA561_9259_44B0_81F6_E8FD340D9A90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TraceDlg.h : header file
//

#include "WebPage.h"
#include "TargetOpt.h"

/////////////////////////////////////////////////////////////////////////////
// CTraceDlg dialog

class CTraceDlg : public CDialog
{
// Construction
public:
	CTraceDlg(CWnd* pParent = NULL);   // standard constructor
	CTraceDlg(int targetID, CString targetName, CWnd* pParent = NULL);

	void CreateTraceMarker();
	void Resize();

// Dialog Data
	//{{AFX_DATA(CTraceDlg)
	enum { IDD = IDD_TRACEDLG };
	CWebBrowser2	m_traceWeb;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTraceDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	POINT old; // Store the dialog's width & height
	CWebPage m_webPage;
	BOOL hasMapFile;
	CTargetOpt targetOpt;
	int targetID;
	CString targetName;
	int nElapse; // 保存定时器的时间间隔

	// Generated message map functions
	//{{AFX_MSG(CTraceDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRACEDLG_H__2A1CA561_9259_44B0_81F6_E8FD340D9A90__INCLUDED_)
