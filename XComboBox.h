#if !defined(AFX_XCOMBOBOX_H__6CB87979_323B_4A91_AF18_F663574B6A20__INCLUDED_)
#define AFX_XCOMBOBOX_H__6CB87979_323B_4A91_AF18_F663574B6A20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XComboBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CXComboBox window

class CXComboBox : public CComboBox
{
// Construction
public:
	CXComboBox();

// Attributes
public:

// Operations
public:
	// …Ë÷√ COMBOBOX µƒƒ⁄»›
	void SetContent(std::map<int, CString> &content);
	void SetContent(const CStringArray &content);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXComboBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CXComboBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XCOMBOBOX_H__6CB87979_323B_4A91_AF18_F663574B6A20__INCLUDED_)
