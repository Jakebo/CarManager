// NewUserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CarManage.h"
#include "NewUserDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewUserDlg dialog


CNewUserDlg::CNewUserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewUserDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewUserDlg)
	//}}AFX_DATA_INIT
}


void CNewUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewUserDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewUserDlg, CDialog)
	//{{AFX_MSG_MAP(CNewUserDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewUserDlg message handlers

void CNewUserDlg::OnOK() 
{
	// TODO: Add extra validation here
	CWnd *childWnd = this->GetWindow(GW_CHILD);
	char cClassName[20];
	CString str; // 保存用户录入的内容
	CString tmpStr;

	// 遍历输入控件以获取录入的内容
	while (childWnd) {
		GetClassName(childWnd->GetSafeHwnd(), cClassName, 255);

		// 如果控件是输入控件(编辑框或是日期选择控件)，则获取内容
		if (strcmp(cClassName, "Edit") == 0) {
			childWnd->GetWindowText(tmpStr);
			str += (tmpStr + ",");
		} else if (strcmp(cClassName, "DTPicker20WndClass") == 0) {
			VARIANT year, month, day;
			
			year.vt		= VT_I4;
			month.vt	= VT_I4;
			day.vt		= VT_I4;

			// 获取选取的时间
			year		= ((CDTPicker *)childWnd)->GetYear();
			month		= ((CDTPicker *)childWnd)->GetMonth();
			day			= ((CDTPicker *)childWnd)->GetDay();

			tmpStr.Format("'%d-%d-%d'",
				year.lVal, month.lVal, day.lVal);

			str += (tmpStr + ",");
		}

		childWnd = childWnd->GetWindow(GW_HWNDNEXT);
	}

	str.Delete(str.GetLength() - 1, 1);

	CDialog::OnOK();
}
