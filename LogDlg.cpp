// LogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CarManage.h"
#include "LogDlg.h"

#include "ADOConn.h"
#include "MD5Checksum.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern ADOConn conn;

/////////////////////////////////////////////////////////////////////////////
// CLogDlg dialog


CLogDlg::CLogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogDlg)
	m_passwd = _T("");
	m_username = _T("");
	m_serverName = _T("");
	//}}AFX_DATA_INIT
}

CString CLogDlg::m_name;
int CLogDlg::m_id;

void CLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogDlg)
	DDX_Control(pDX, IDCANCEL, m_cancelBtn);
	DDX_Control(pDX, IDOK, m_okBtn);
	DDX_Control(pDX, IDC_STATIC3, m_msgLab);
	DDX_Control(pDX, IDC_EDIT2, m_pwBox);
	DDX_Control(pDX, IDC_EDIT1, m_userNameBox);
	DDX_Control(pDX, IDC_STATIC2, m_pwLab);
	DDX_Control(pDX, IDC_STATIC1, m_userNameLab);
	DDX_Text(pDX, IDC_EDIT2, m_passwd);
	DDX_Text(pDX, IDC_EDIT1, m_username);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogDlg, CDialog)
	//{{AFX_MSG_MAP(CLogDlg)
	ON_BN_CLICKED(IDC_BUTTON4, OnMinSize)
	ON_BN_CLICKED(IDC_BUTTON3, OnCloseDlg)
	ON_BN_CLICKED(IDOK, OnLoging)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogDlg message handlers

void CLogDlg::OnMinSize() 
{
	// TODO: Add your control notification handler code here
	this->ShowWindow(SW_MINIMIZE);
}

void CLogDlg::OnCloseDlg() 
{
	// TODO: Add your control notification handler code here
	this->EndDialog(0);
	::exit(0);
}

void CLogDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	this->EndDialog(0);
	::exit(0);

	CDialog::OnCancel();
}

void CLogDlg::OnLoging() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	// 将用户输入的密码转换为MD5码
	m_passwd.ReleaseBuffer();
	m_passwd = CMD5Checksum::GetMD5((BYTE*)(const char*)m_passwd.GetBuffer(0),
				m_passwd.GetLength());

	CString strSql = "select id,name,password from master where name='" +
					m_username + "' and password='" + m_passwd + "'";

	try {
		_RecordsetPtr &pRst = conn.GetRecordSet(strSql);

		if (!pRst->adoEOF) {
			m_name = m_username;
			m_id = pRst->GetCollect("id").lVal;

			this->EndDialog(0);
			this->OnOK();
		} else {
			MessageBox("用户名或密码不正确");
		}
	}
	catch (CADOException e) {
		MessageBox(e.m_strMessage);
	}
}

// GetName : return loging master's name
CString &CLogDlg::GetName()
{
	return m_name;
}

// GetID : return loging master's id
int &CLogDlg::GetID()
{
	return m_id;
}

BOOL CLogDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->SetWindowText("基于电子地图和 GPS 的车辆信息管理--登录");

	SetControlVisible(FALSE);

	SetTimer(5, 500, NULL);

	// 为方便调试，设置默认值，调试完成之后要删除
	m_username = "admin";
	m_passwd = "1";
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLogDlg::SetControlVisible(BOOL visible)
{
	if (!visible) {
		m_userNameLab.ShowWindow(SW_HIDE);
		m_pwLab.ShowWindow(SW_HIDE);
		m_userNameBox.ShowWindow(SW_HIDE);
		m_pwBox.ShowWindow(SW_HIDE);
		m_okBtn.ShowWindow(SW_HIDE);
		m_cancelBtn.ShowWindow(SW_HIDE);
		m_msgLab.ShowWindow(SW_SHOW);
	} else {
		m_userNameLab.ShowWindow(SW_SHOW);
		m_pwLab.ShowWindow(SW_SHOW);
		m_userNameBox.ShowWindow(SW_SHOW);
		m_pwBox.ShowWindow(SW_SHOW);
		m_okBtn.ShowWindow(SW_SHOW);
		m_cancelBtn.ShowWindow(SW_SHOW);
		m_msgLab.ShowWindow(SW_HIDE);
	}
}

void CLogDlg::ConnectDataBase()
{
	// TODO: Add your message handler code here and/or call default
	// 连接数据库字符串
	CString strConnect = "Provider=SQLOLEDB.1; \
			  Integrated Security=SSPI; \
			  Persist Security Info=False; \
			  Initial Catalog=CarsManage; \
			  Data Source=."; //SL-20111023YXOP";
	try {
		if (!conn.OnInitADOConn(strConnect)) {
			m_msgLab.SetWindowText("服务器连接失败......");
		} else {
			SetControlVisible(TRUE);
		}
	}
	catch (CADOException e) {
		TRACE(e.m_strMessage);
	}
}

void CLogDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(5);
	ConnectDataBase();

	CDialog::OnTimer(nIDEvent);
}
