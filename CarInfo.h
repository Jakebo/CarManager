#if !defined(AFX_CARINFO_H__DD7FB1FB_ECE7_4569_9A1E_64D1C08A08E1__INCLUDED_)
#define AFX_CARINFO_H__DD7FB1FB_ECE7_4569_9A1E_64D1C08A08E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CarInfo.h : header file
//
#include "afxtempl.h"

#include "SizingDialog.h"
#include "XCListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCarInfo dialog

class CCarInfo : public CSizingDialog
{
// Construction
public:
	CCarInfo(CWnd* pParent = NULL);   // standard constructor

public:
	void SetListSel(const CString &coding); // 设置列表框中的当前选中项
	void FillingInfo(_RecordsetPtr &pRst); // 填充信息
	void AddCtrl(); // 将控件加入到自动适应大小

// Dialog Data
	//{{AFX_DATA(CCarInfo)
	enum { IDD = IDD_CARINFO };
	CXCListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCarInfo)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCarInfo)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnDblclkInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CARINFO_H__DD7FB1FB_ECE7_4569_9A1E_64D1C08A08E1__INCLUDED_)
