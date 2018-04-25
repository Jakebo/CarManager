#if !defined(AFX_CARDATADLG_H__736BD84E_8545_4F55_B63F_930D4D8D4510__INCLUDED_)
#define AFX_CARDATADLG_H__736BD84E_8545_4F55_B63F_930D4D8D4510__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CarDataDlg.h : header file
//
#include "SizingDialog.h"
#include "CarOpt.h"
#include "MyTabCtrl.h"
#include "XCListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCarDataDlg dialog

class CCarDataDlg : public CSizingDialog
{
// Construction
// 注意，默认的构造函数已经被删掉
public:
	CCarDataDlg(CString &purviewCoding, CWnd *pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CCarDataDlg)
	enum { IDD = IDD_CARDATADLG };
	CMyTabCtrl	m_manageTab;
	CEdit	m_searchText;
	CXCListCtrl	m_carLst;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCarDataDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ModuleSetup(CString &purviewCoding);
	void AddCtrl();
	void FillCarLst(_RecordsetPtr &pRst);
	// 更新某一行的内容
	void UpdateRowContent(const int &index,
		const CStringArray &content);

	// Generated message map functions
	//{{AFX_MSG(CCarDataDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSearch();
	afx_msg void OnUpdate();
	afx_msg void OnAddNew();
	afx_msg void OnModify();
	afx_msg void OnDel();
	afx_msg void OnDblclkCarLst(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

// Attribute
	CString &purviewCoding;
	CCarOpt carOpt;
	CToolTipCtrl m_toolTip;

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CARDATADLG_H__736BD84E_8545_4F55_B63F_930D4D8D4510__INCLUDED_)
