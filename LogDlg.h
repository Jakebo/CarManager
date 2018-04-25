#if !defined(AFX_LOGDLG_H__CA1D6CD2_E99C_4E2F_9CE0_51D6EEC34DCF__INCLUDED_)
#define AFX_LOGDLG_H__CA1D6CD2_E99C_4E2F_9CE0_51D6EEC34DCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogDlg.h : header file
//
#define CONNECTTIMER	5
/////////////////////////////////////////////////////////////////////////////
// CLogDlg dialog

class CLogDlg : public CDialog
{
// Construction
public:
	CLogDlg(CWnd* pParent = NULL);   // standard constructor

	void ConnectDataBase();
	void SetControlVisible(BOOL visible);

// Dialog Data
	//{{AFX_DATA(CLogDlg)
	enum { IDD = IDD_LOGDLG };
	CButton	m_connect;
	CButton	m_cancelBtn;
	CButton	m_okBtn;
	CStatic	m_msgLab;
	CEdit	m_pwBox;
	CEdit	m_userNameBox;
	CStatic	m_pwLab;
	CStatic	m_userNameLab;
	CString	m_passwd;
	CString	m_username;
	CString	m_serverName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogDlg)
	public:
	//virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLogDlg)
	afx_msg void OnMinSize();
	afx_msg void OnCloseDlg();
	virtual void OnCancel();
	afx_msg void OnLoging();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

public:
	static CString &GetName();
	static int &GetID();

private:
	static CString m_name;
	static int m_id;

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGDLG_H__CA1D6CD2_E99C_4E2F_9CE0_51D6EEC34DCF__INCLUDED_)
