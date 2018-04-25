// �Լ�д��TabCtrl�ؼ���ֻ��ѱ�ǩ�������ڴ��������ɡ�
// Ŀǰ�߱��Ĺ����У�
//		1. TAB �Ĵ�С�ı�ʱ�ɵ��� ONSIZE �������������ڴ�С����Ӧ
//		2. ��ǩҳ��ѡ��״̬�������ı�ʱ��Ӧ�����Զ���ʾ������
// ����ע��������У�
//		  1. �����𴰿ڵ����١�
//		  2.InsertLabel() ������dlg��������Ϊ��
// �Ժ��ٸĽ�
//												by:�Ʋ���
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
	// ��ӱ�ǩ
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
