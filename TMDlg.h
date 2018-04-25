#if !defined(AFX_TMDLG_H__BE73109F_71EE_4729_9927_50852E81D49E__INCLUDED_)
#define AFX_TMDLG_H__BE73109F_71EE_4729_9927_50852E81D49E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TMDlg.h : header file
//
#include "TargetOpt.h"
#include "XCListCtrl.h"
#include "SizingDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CTMDlg dialog

class CTMDlg : public CSizingDialog
{
// Construction
public:
	CTMDlg(CString &purviewCoding, CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CTMDlg)
	enum { IDD = IDD_TMDLG };
	CXCListCtrl	m_list;
	CString	m_searchText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTMDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ModuleSetup(CString &purviewCoding);
	void AddCtrl(); // ���ؼ���ӵ�����Ӧ��С��
	void InitInterface(); // ��ʼ�û�����
	void FillLst(); // ����ն��б�
	void AddNewRow(const CStringArray &content); // ���б������һ��
	void GetRowContent(CStringArray &content); // ����һ��Ҫ���ȡѡ��ĳ��������
	void SetRowContent(const int row,
						const CStringArray &content); // ����ĳ�е�����

// Attiube
private:
	CTargetOpt targetOpt;
	CString &m_purview;
	CToolTipCtrl m_toolTip;

	// Generated message map functions
	//{{AFX_MSG(CTMDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnNewTarget();
	afx_msg void OnModi();
	afx_msg void OnDel();
	afx_msg void OnSearch();
	afx_msg void OnUpdate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TMDLG_H__BE73109F_71EE_4729_9927_50852E81D49E__INCLUDED_)
