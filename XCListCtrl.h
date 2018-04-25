#if !defined(AFX_XCLISTCTRL_H__629C63A0_5040_4A67_8171_105DAFC9C9D1__INCLUDED_)
#define AFX_XCLISTCTRL_H__629C63A0_5040_4A67_8171_105DAFC9C9D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XCListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CXCListCtrl window

class CXCListCtrl : public CListCtrl
{
// Construction
public:
	CXCListCtrl();

// Attributes
public:

// Operations
public:
	void GetCurSelContent(CStringArray &content); // ��ȡ��ǰ�б�ѡ���е�����
	void AddToLstTail(CStringArray &content); // ��ָ����CStringArray��ӵ��б�β
	void SetListContent(_Recordset &pRst); // ��ָ���ļ�¼����䵽�б���
	CString ConvertVariantToString(const _variant_t &var);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXCListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXCListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CXCListCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XCLISTCTRL_H__629C63A0_5040_4A67_8171_105DAFC9C9D1__INCLUDED_)
