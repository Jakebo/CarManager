// NewRoleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CarManage.h"
#include "NewRoleDlg.h"
#include "ADOConn.h"
#include "LogDlg.h"
#include "RoleOpt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewRoleDlg dialog


CNewRoleDlg::CNewRoleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewRoleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewRoleDlg)
	m_rName = _T("");
	//}}AFX_DATA_INIT
}


void CNewRoleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewRoleDlg)
	DDX_Text(pDX, IDC_EDIT1, m_rName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewRoleDlg, CDialog)
	//{{AFX_MSG_MAP(CNewRoleDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewRoleDlg message handlers

void CNewRoleDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

	if (m_rName.IsEmpty()) {
		MessageBox("角色名不能为空", "提示信息", MB_ICONWARNING);
		return;
	}

	CRoleOpt ro;
	int cmID = CLogDlg::GetID();
	CTime now = CTime::GetCurrentTime();

	ro.AddRole(m_rName, cmID, now.Format("%y-%m-%d   %H:%M:%S"));
	
	CDialog::OnOK();
}
