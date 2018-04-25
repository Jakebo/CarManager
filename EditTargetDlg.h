#if !defined(AFX_EDITTARGETDLG_H__6983AFB7_6615_4E87_A56C_1BC020D2A81D__INCLUDED_)
#define AFX_EDITTARGETDLG_H__6983AFB7_6615_4E87_A56C_1BC020D2A81D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditTargetDlg.h : header file
//

#include "TargetOpt.h"
#include "XComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CEditTargetDlg dialog

class CEditTargetDlg : public CDialog
{
// Construction
public:
	CEditTargetDlg(CWnd* pParent = NULL);   // standard constructor
	CEditTargetDlg(const CStringArray &content,
					CWnd *pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CEditTargetDlg)
	enum { IDD = IDD_EDITTARGETDLG };
	CComboBox	m_groupcombox;
	CComboBox	m_denycombox;
	CXComboBox	m_teamcombox;
	CString	m_pw;
	CString	m_rpw;
	CString	m_coding;
	CString	m_targetName;
	CString	m_contract;
	CString	m_phone;
	CString	m_comment;
	CString	m_deny;
	CString	m_group;
	CString	m_team;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditTargetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitInterface(); // 初始用户界面
	void InitCombox(); // 初始 COMBOBOX 控件
	void GetContent(CStringArray &content); // 获取用户在窗口内容
	void GetUpdateText(CString &text);
	BOOL DetectCondition(); // 判断用户的输入是否符合条件

public:
	CStringArray &GetContent();
	void SetContent(const CStringArray &content);

private:
	CTargetOpt targetOpt;
	CStringArray content;
	BOOL m_bNew;

	// Generated message map functions
	//{{AFX_MSG(CEditTargetDlg)
	afx_msg void OnSave();
	afx_msg void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITTARGETDLG_H__6983AFB7_6615_4E87_A56C_1BC020D2A81D__INCLUDED_)
