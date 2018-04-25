// CarManageView.h : interface of the CCarManageView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CARMANAGEVIEW_H__FE8D0271_31AA_43A2_9AC0_E97FCF9AA9F6__INCLUDED_)
#define AFX_CARMANAGEVIEW_H__FE8D0271_31AA_43A2_9AC0_E97FCF9AA9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <iostream>
#include <map>

#include "MyTabCtrl.h"
#include "RoleOpt.h"

////////////////////////////////////////////////////////
// Dialog type enum

enum DialogType {
	MAPDLG, CARDATADLG, ROLEMANAGEDLG
};

////////////////////////////////////////////////////////
// CCarManageView
class CCarManageDoc;
class CMapDlg;

class CCarManageView : public CFormView
{
protected: // create from serialization only
	CCarManageView();
	DECLARE_DYNCREATE(CCarManageView)

public:
	//{{AFX_DATA(CCarManageView)
	enum { IDD = IDD_CARMANAGE_FORM };
	//}}AFX_DATA

// Attributes
public:
	CCarManageDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCarManageView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCarManageView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void ModuleSetup(CString &purviewCoding);

// Generated message map functions
protected:
	//{{AFX_MSG(CCarManageView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnModipw();
	afx_msg void OnRelogin();
	afx_msg void OnExit();
	//}}AFX_MSG

private:
	CMyTabCtrl	m_tab; // ·â×°ºóµÄTabCtrl
	std::map<DialogType, CDialog*> m_dlgMap; // Already opened windows map
	std::map<int, DialogType> m_tabDlgMap; // Opening tab index & windows map
	std::map<DialogType, int> m_dlgTabMap; // opening windows & tab index map
	short m_curSelIndex;
	CRoleOpt ro;

	DECLARE_MESSAGE_MAP()
};

extern CMapDlg *mapDlg;

#ifndef _DEBUG  // debug version in CarManageView.cpp
inline CCarManageDoc* CCarManageView::GetDocument()
   { return (CCarManageDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CARMANAGEVIEW_H__FE8D0271_31AA_43A2_9AC0_E97FCF9AA9F6__INCLUDED_)
