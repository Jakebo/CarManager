// 自己写的TabCtrl控件，只需把标签名跟窗口传进来即可。
// 目前具备的功能有：
//		1. TAB 的大小改变时可调用 ONSIZE 函数来调整窗口大小来适应
//		2. 标签页的选中状态来发生改变时对应窗口自动显示与隐藏
// 但就注意的问题有：
//		  1. 不负责窗口的销毁。
//		  2.InsertLabel() 函数的dlg参数不能为空
// 以后再改进
//												by:黄昌军
#if !defined(AFX_MYTABCTRL_H__11F9D4B2_D935_416C_98B9_06E8ECC1E158__INCLUDED_)
#define AFX_MYTABCTRL_H__11F9D4B2_D935_416C_98B9_06E8ECC1E158__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyTabCtrl.h : header file
//
#include <map>

/////////////////////////////////////////////////////////////////////////////
// CMyTabCtrl window

class CMyTabCtrl : public CTabCtrl
{
// Construction
public:
	CMyTabCtrl();

// Attributes
private:
	std::map<int, CDialog *> m_dlgMap;
	short m_curSelIndex;
	short m_dlgCount;

// Operations
public:
	// 添加标签
	void InsertLabel(const CString &labelName, CDialog *dlg);
	std::map<int, CDialog *>& GetDlgMap();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyTabCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyTabCtrl();

	// Generated message map functions
public:
	//{{AFX_MSG(CMyTabCtrl)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTABCTRL_H__11F9D4B2_D935_416C_98B9_06E8ECC1E158__INCLUDED_)
