#if !defined(AFX_NEWROLEDLG_H__716F156F_67E1_47CB_BD0D_15E48B5774E6__INCLUDED_)
#define AFX_NEWROLEDLG_H__716F156F_67E1_47CB_BD0D_15E48B5774E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewRoleDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewRoleDlg dialog

class CNewRoleDlg : public CDialog
{
// Construction
public:
	CNewRoleDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewRoleDlg)
	enum { IDD = IDD_NEWROLEDLG };
	CString	m_rName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewRoleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewRoleDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWROLEDLG_H__716F156F_67E1_47CB_BD0D_15E48B5774E6__INCLUDED_)
