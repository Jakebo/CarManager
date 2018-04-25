// EditTargetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CarManage.h"
#include "EditTargetDlg.h"
#include "MD5Checksum.h"
#include "TeamOpt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditTargetDlg dialog


CEditTargetDlg::CEditTargetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditTargetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditTargetDlg)
	m_pw = _T("");
	m_rpw = _T("");
	m_coding = _T("");
	m_targetName = _T("");
	m_contract = _T("");
	m_phone = _T("");
	m_comment = _T("");
	m_deny = _T("");
	m_group = _T("");
	m_bNew = TRUE;
	m_team = _T("");
	//}}AFX_DATA_INIT
}

CEditTargetDlg::CEditTargetDlg(const CStringArray &content,
							   CWnd *pParent /*= NULL*/)
   : CDialog(CEditTargetDlg::IDD, pParent)
{
	m_bNew = FALSE;
	this->content.Copy(content);
}

void CEditTargetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditTargetDlg)
	DDX_Control(pDX, IDC_COMBO4, m_groupcombox);
	DDX_Control(pDX, IDC_COMBO1, m_denycombox);
	DDX_Control(pDX, IDC_COMBO3, m_teamcombox);
	DDX_Text(pDX, IDC_EDIT6, m_pw);
	DDX_Text(pDX, IDC_EDIT8, m_rpw);
	DDX_Text(pDX, IDC_EDIT1, m_coding);
	DDX_Text(pDX, IDC_EDIT2, m_targetName);
	DDX_Text(pDX, IDC_EDIT4, m_contract);
	DDX_Text(pDX, IDC_EDIT5, m_phone);
	DDX_Text(pDX, IDC_RICHEDIT1, m_comment);
	DDX_CBString(pDX, IDC_COMBO1, m_deny);
	DDX_CBString(pDX, IDC_COMBO4, m_group);
	DDX_CBString(pDX, IDC_COMBO3, m_team);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditTargetDlg, CDialog)
	//{{AFX_MSG_MAP(CEditTargetDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnSave)
	ON_BN_CLICKED(IDC_BUTTON3, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditTargetDlg message handlers
BOOL CEditTargetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (!m_bNew) {
		SetContent(content);
	}

	InitInterface();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// InitInterface() : 初始用户界面
void CEditTargetDlg::InitInterface()
{
	if (m_bNew) {
		// 初始编码框
		CString coding;
		CString strCou;
		CTime	date  = CTime::GetCurrentTime();
		int		count = targetOpt.GetTCByDate(date);

		strCou.Format("%04d", count);
		coding = date.Format("%Y%m%d");
		coding += strCou;

		GetDlgItem(IDC_EDIT1)->SetWindowText(coding);

		SetWindowText("新增终端");
	} else {
		SetWindowText("编辑终端");
	}

	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	
	// 初始 COMBOBOX 的内容
	InitCombox();
}

// InitCombox() : 初始 COMBOBOX	控件内容
void CEditTargetDlg::InitCombox()
{
	std::map<int, CString> list;
	CTeamOpt teamOpt;

	teamOpt.GetTeam(list);
	m_teamcombox.SetContent(list);
	
	if (!m_bNew) {
		m_teamcombox.ShowDropDown(TRUE);
		m_teamcombox.ShowDropDown(FALSE);
	} else {
	}
}

// GetContent() : 获取用户在窗口内录入的内容
// content : 保存并返回获取得到的内容
void CEditTargetDlg::GetContent(CStringArray &content)
{
	CWnd *pWnd = GetWindow(GW_CHILD);
	CString text;
	int count;

	content.RemoveAll();
	for (count = 0; count < 8; ++count) {
		if (IDC_COMBO3 != ::GetDlgCtrlID(pWnd->GetSafeHwnd())) {
			pWnd->GetWindowText(text);
			content.Add(text);
		} else {
			int data = m_teamcombox.GetItemData(m_teamcombox.GetCurSel());
			text.Format("%d", data);
			content.Add(text);
		}

		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}

	// 如果不是新增终端资料的话不用更新登记时间，这个更新
	// 是指在终端列表里面的更新，不是指数据库
	// 而且更新数据库不是在这里获取数据的
	if (m_bNew) {
		// 获取密码并使用 MD5 加密
		m_pw.ReleaseBuffer();
		m_pw = CMD5Checksum::GetMD5((BYTE*)(const char*)m_pw.GetBuffer(0),
					m_pw.GetLength());
		content.Add(m_pw);

		// 获取当前时间
		CTime time = CTime::GetCurrentTime();
		content.Add(time.Format("%Y-%m-%d"));
	}
}

// GetUpdateText() : 获取更新文本
// updateText : 保存得到的文本
void CEditTargetDlg::GetUpdateText(CString &text)
{
	UpdateData();
	CString data;

	text = ""; // 初始 TEXT 为空先
	text += "name='" + m_targetName + "',contract='" + m_contract +
			"',phoneno='" + m_phone + "',[deny]='" + m_deny + "',cargroup='";

	data.Format("%d", m_teamcombox.GetItemData(m_teamcombox.GetCurSel()));

	text += data + "',usergroup='" + m_group + "',comment='" + m_comment + "'";

	if (m_pw.GetLength() < 1) {
		m_pw.ReleaseBuffer();
		m_pw = CMD5Checksum::GetMD5((BYTE*)(const char*)m_pw.GetBuffer(0),
					m_pw.GetLength());
		text += ",password='" + m_pw + "'";
	}
}

// DetectCondition() : 判断用户输入的内容是否符合条件
// 返回值 : 如果符合返回 TRUE，否则返回 FALSE
BOOL CEditTargetDlg::DetectCondition()
{
	UpdateData();

	if (m_pw.Compare(m_rpw) != 0) {
		MessageBox("两次输入的密码不正确", "提示信息", MB_ICONWARNING);
		return FALSE;
	}

	return TRUE;
}

// GetContent() : 返回窗口内容
// 返回值 : CSTRINGARRAY
CStringArray &CEditTargetDlg::GetContent()
{
	if (m_bNew) {
		content.RemoveAt(content.GetSize() - 2);
	}

	content.SetAt(5, m_team);

	return content;
}

// SetContent() : 设置窗口的内容
// content : 将要设置窗口的内容
void CEditTargetDlg::SetContent(const CStringArray &content)
{
	int size = content.GetSize();
	CWnd *pWnd = GetWindow(GW_CHILD);

	for (int index = 0; index < size; ++index) {
		pWnd->SetWindowText(content.GetAt(index));
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}
}

void CEditTargetDlg::OnSave() 
{
	// TODO: Add your control notification handler code here
	if (!DetectCondition()) {
		return;
	}

	if (m_bNew) { // 如果是新增终端资料
		GetContent(content);
		targetOpt.AddTarget(content);
	} else { // 如果是编辑终端资料
		CString updateText;
		GetUpdateText(updateText);
		targetOpt.UpdateTarget(m_coding, updateText);
		GetContent(content);
	}

	CDialog::OnOK();
}

void CEditTargetDlg::OnCancel()
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}
