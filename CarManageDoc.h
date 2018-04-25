// CarManageDoc.h : interface of the CCarManageDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CARMANAGEDOC_H__D0FAA023_C20E_4E27_A850_6AEDDE93C861__INCLUDED_)
#define AFX_CARMANAGEDOC_H__D0FAA023_C20E_4E27_A850_6AEDDE93C861__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCarManageDoc : public CDocument
{
protected: // create from serialization only
	CCarManageDoc();
	DECLARE_DYNCREATE(CCarManageDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCarManageDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCarManageDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCarManageDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CARMANAGEDOC_H__D0FAA023_C20E_4E27_A850_6AEDDE93C861__INCLUDED_)
