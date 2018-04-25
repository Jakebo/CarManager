// CarManageDoc.cpp : implementation of the CCarManageDoc class
//

#include "stdafx.h"
#include "CarManage.h"

#include "CarManageDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCarManageDoc

IMPLEMENT_DYNCREATE(CCarManageDoc, CDocument)

BEGIN_MESSAGE_MAP(CCarManageDoc, CDocument)
	//{{AFX_MSG_MAP(CCarManageDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCarManageDoc construction/destruction

CCarManageDoc::CCarManageDoc()
{
	// TODO: add one-time construction code here

}

CCarManageDoc::~CCarManageDoc()
{
}

BOOL CCarManageDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCarManageDoc serialization

void CCarManageDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCarManageDoc diagnostics

#ifdef _DEBUG
void CCarManageDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCarManageDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCarManageDoc commands
