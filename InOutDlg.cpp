// InOutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CarManage.h"
#include "InOutDlg.h"
#include "UserOpt.h"
#include "LogDlg.h"
#include "DeptOpt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInOutDlg dialog


CInOutDlg::CInOutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInOutDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInOutDlg)
	m_comment = _T("");
	m_driverphone = _T("");
	m_usetime = 0;
	m_backtime = 0;
	m_phone = _T("");
	//}}AFX_DATA_INIT
}

CInOutDlg::CInOutDlg(BOOL bInOut, CWnd *pParent /*=NULL*/)
	: CDialog(CInOutDlg::IDD, pParent), m_bInOut(bInOut)
{
}

CInOutDlg::CInOutDlg(BOOL bInOut, std::vector<std::string> &content,
					 CWnd *pParent /*= NULL*/)
	: CDialog(CInOutDlg::IDD, pParent), m_bInOut(bInOut), content(content)
{
}

void CInOutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInOutDlg)
	DDX_Control(pDX, IDC_COMBO4, m_usedept);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_backctrl);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_usectrl);
	DDX_Control(pDX, IDC_COMBO7, m_whobackcombox);
	DDX_Control(pDX, IDC_COMBO5, m_whopastcombox);
	DDX_Control(pDX, IDC_COMBO6, m_whooutcombox);
	DDX_Control(pDX, IDC_COMBO3, m_usercombox);
	DDX_Control(pDX, IDC_COMBO1, m_numbercombox);
	DDX_Control(pDX, IDC_COMBO2, m_drivercombox);
	DDX_Text(pDX, IDC_COMMENTEDIT, m_comment);
	DDX_Text(pDX, IDC_EDIT3, m_driverphone);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_usetime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_backtime);
	DDX_Text(pDX, IDC_EDIT4, m_phone);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInOutDlg, CDialog)
	//{{AFX_MSG_MAP(CInOutDlg)
	ON_BN_CLICKED(IDOK, OnSave)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnSelchangeDriver)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeNumber)
	ON_CBN_SELCHANGE(IDC_COMBO3, OnSelchangeUser)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInOutDlg message handlers

BOOL CInOutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitInterface();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// InitInterface() : ��ʼ������
void CInOutDlg::InitInterface() {
	if (m_bInOut) { // ����������ɳ�
		SetWindowText("�ɳ�"); // ���ô��ڱ���
		// ����һ��Ψһ�ı���
		CTime		curDate	= CTime::GetCurrentTime();
		std::string date	= curDate.Format("%Y-%m-%d");
		int			num		= m_ioOpt.QueryRecordNumByDate(date);
		CString		coding;
		CString		numStr;

		numStr.Format("%04d", num);
		coding = curDate.Format("%Y%m%d") + numStr;

		GetDlgItem(IDC_CODING)->SetWindowText(coding);
		// �����ճ��˱༭��
		GetDlgItem(IDC_BACKLAB)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO7)->ShowWindow(SW_HIDE);
		// ���ó�ʼʱ��
		m_usectrl.SetTime(&curDate);
		m_backctrl.SetTime(&curDate);
	} else {
		SetWindowText("�ճ�"); // ���ô��ڱ���
		SetContent(content); // ���ô�������
		m_numbercombox.EnableWindow(FALSE); // ���ó��ƺŲ��ɱ༭
	}

	InitCombox();
}

// InItCombox() : ��ʼ COMBOBOX �ؼ�
void CInOutDlg::InitCombox()
{
	_RecordsetPtr pRst;
	std::map<int, CString> list;

	// ���ó��ƺŵ� COMBOBOX �ؼ�������
	m_carOpt.GetNumber(list);
	m_numbercombox.SetContent(list);

	// ���ü�ʻԱ COMBOBOX �ؼ�������
	list.clear(); // �ǵ�Ҫ��գ���Ȼ������ǰ������
	m_driverOpt.QueryDriver(list);
	m_drivercombox.SetContent(list);

	// �����ó��ˡ��ɳ��� COMBOBOX �ؼ�������
	CUserOpt userOpt;
	list.clear();
	userOpt.GetUser(list);
	m_whobackcombox.SetContent(list);
	m_usercombox.SetContent(list);
	m_whooutcombox.SetContent(list);
	m_whopastcombox.SetContent(list);

	// �����ó����� COMBOBOX �ؼ�����
	CDeptOpt deptOpt;
	list.clear();
	deptOpt.GetDept(list);
	m_usedept.SetContent(list);
	
	if (!m_bInOut) { // �����ճ��� COMBOBOX �ؼ�������
		m_whobackcombox.SetWindowText(CLogDlg::GetName());
	} else { // ������ɳ�����Ĭ��ѡ��
		m_whooutcombox.SetWindowText(CLogDlg::GetName());
		m_numbercombox.SetCurSel(0);
		m_drivercombox.SetCurSel(0);
		OnSelchangeDriver();
	}

	// ���ó������͡�Ʒ���ͺ�
}

void CInOutDlg::OnSelchangeNumber() 
{
	// TODO: Add your control notification handler code here
	std::string driver;
	int nIndex	= m_numbercombox.GetCurSel(); // ��ȡ���ƺ�����
	int id		= m_numbercombox.GetItemData(nIndex); // ��ȡ���ƺŶ�ӦID

	m_carOpt.GetDriverById(id, driver); // ��ȡ���ƺŶ�Ӧ�ļ�ʻԱ

	nIndex = m_drivercombox.FindString(-1, driver.data()); // ���Ҷ�Ӧ��ʻԱ����
	
	m_drivercombox.SetCurSel(nIndex); // ѡ�ж�Ӧ�ļ�ʻԱ
	OnSelchangeDriver(); // ������Ϊʲô�����Զ������ı�ѡ���¼�?
}

void CInOutDlg::OnSelchangeDriver() 
{
	// TODO: Add your control notification handler code here
	std::string phone;
	UINT nIndex = m_drivercombox.GetCurSel(); // ��ȡ��ʻԱ����
	int  id = m_drivercombox.GetItemData(nIndex); // ��ȡ��ʻԱ ID

	m_driverOpt.QueryPhoById(id, phone); // ��ȡ��ʻԱ�绰
	m_driverphone = phone.data(); // ���ü�ʻԱ�绰�ؼ�������

	GetDlgItem(IDC_EDIT3)->SetWindowText(phone.data());; // ���¼�ʻԱ�绰�ؼ�����
}

void CInOutDlg::OnSave() 
{
	// TODO: Add your control notification handler code here
	if (m_bInOut) {
		if (!NewOutCar()) {
			return;
		}
	} else {
		if (!BackCar()) {
			return;
		}
	}

	CDialog::OnOK();
}

// NewOutCar() : �½�һ���ɳ���¼
BOOL CInOutDlg::NewOutCar()
{
	if (!DetectCondition()) {
		return FALSE;
	}

	GetTextContent();

	m_ioOpt.NewOutCarRecord(content); // �½��ɳ���¼

	// ����δ���ı���Ϊ�������ɳ���¼����ʾ
	std::vector<std::string>::iterator it = content.end();
	CString dept;
	it -= 3;
	*it = "δ��";
	// ���ò����ı�
	it -= 10;
	m_usedept.GetWindowText(dept);
	*it = dept;

	return TRUE;
}

// BackCar() : �ճ�
BOOL CInOutDlg::BackCar()
{
	UpdateData();

	// �ж�ѡ��ʱ���Ƿ���ϳ���
	CTimeSpan	tmSp = m_usetime - m_backtime;
	if (tmSp.GetTotalSeconds() > 0) {
		MessageBox("ʹ��ʱ�䲻Ӧ���ڹ��Ʒ���ʱ��", "��ʾ��Ϣ", MB_ICONWARNING);

		return FALSE;
	}

	GetTextContent();
	m_ioOpt.InComeCar(content); // �ճ�

	// ��һ��������ú� CONTENT��Ϊ�������ɳ���¼/�ճ���¼����ʾ
	std::vector<std::string>::iterator it = content.end();
	CString name;
	it -= 3;
	*(it++) = "����";
	m_whobackcombox.GetWindowText(name);
	*it = name.GetBuffer(0);
	// ���ò����ı�
	it -= 11; // ��Ϊǰ��++�ˣ�����������11
	m_usedept.GetWindowText(name);
	*it = name;

	return TRUE;
}

// DetectCondition() : ����û�����������Ƿ���������
// ����ֵ : ������������������� TRUE�����򷵻� FALSE
BOOL CInOutDlg::DetectCondition()
{
	UpdateData();

	// �ж�ѡ��ʱ���Ƿ���ϳ���
	CTimeSpan	tmSp = m_usetime - m_backtime;
	if (tmSp.GetTotalSeconds() > 0) {
		MessageBox("ʹ��ʱ�䲻Ӧ���ڹ��Ʒ���ʱ��", "��ʾ��Ϣ", MB_ICONWARNING);

		return FALSE;
	}

	// �ж�ѡ���ʱ��θ��Ѿ��ɳ���ʱ����Ƿ��ͻ
	CString number;
	CString usetime;
	CString backtime;
	std::string conflict;

	m_numbercombox.GetWindowText(number);
	usetime  = m_usetime.Format("%Y-%m-%d");
	backtime = m_backtime.Format("%Y-%m-%d");
	if (!m_ioOpt.DetectLicit(number.GetBuffer(0),
							usetime.GetBuffer(0),
							backtime.GetBuffer(0),
							conflict))
	{
		MessageBox("���ƺ�Ϊ: " + number + " �ĳ�����ʱ��� " + usetime + 
				   "����" + backtime + " ����ó��������ɳ�ʱ����г�ͻ" + 
				   conflict.data(),
				   "��ʾ��Ϣ", MB_ICONWARNING);

		return FALSE;
	}

	return TRUE;
}

// GetTextContent() : ��ȡ���ڵ��ı�����
void CInOutDlg::GetTextContent()
{
	CWnd		*pWnd = GetWindow(GW_CHILD);
	char		cClassName[255] = { 0 };
	CString		text;

	// �������ڿؼ��Ի�ȡ��������
	content.clear();
	while (pWnd) {
		GetClassName(pWnd->GetSafeHwnd(), cClassName, 255);
		// ����ؼ�Ϊ STATIC �� BUTTON �ؼ�������
		// �����ȡ�ؼ�����
		if ((strcmp(cClassName, "Static") == 0) ||
			(strcmp(cClassName, "Button") == 0)) {
		} else {
			pWnd->GetWindowText(text);
			content.push_back(text.GetBuffer(0));
		}

		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}

	// ��β����һ�������� CONTENT
	std::vector<std::string>::iterator it = content.end();
	it -= 2;

	CTime curTime = CTime::GetCurrentTime(); // ��ȡ��ǰʱ��
	text = curTime.Format("%Y-%m-%d");
	content.insert(it, text.GetBuffer(0)); // ���뵱ǰʱ��
	it = content.end();
	it -= 2;
	m_bInOut ? content.insert(it, "0") : // �����Ƿ����ճ��������ɳ�Ĭ��Ϊ0
			   content.insert(it, "1"); // ������ճ�����Ϊ1

	CString data;
	data.Format("%d", m_usedept.GetItemData(m_usedept.GetCurSel()));
	it -= 10;
	*it = data;
}

// GetContent() : ��ȡ��������
// ����ֵ : ���ش��ڵ�����
const std::vector<std::string> &CInOutDlg::GetContent()
{
	return this->content;
}

// SetContent() : ���ô��ڵ�����
// cot : ��Ҫ���õĴ�������
void CInOutDlg::SetContent(std::vector<std::string> &cot)
{
	std::vector<std::string>::iterator begin = cot.begin();
	std::vector<std::string>::iterator end   = cot.end();
	CWnd *pWnd = this->GetWindow(GW_CHILD);

	while (begin != end) {
		pWnd->SetWindowText((*begin).data());
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
		++begin;
	}
}

void CInOutDlg::OnSelchangeUser() 
{
	// TODO: Add your control notification handler code here
	CUserOpt userOpt;
	int nIndex  = m_usercombox.GetCurSel();
	int id		= m_usercombox.GetItemData(nIndex);

	m_phone = userOpt.GetUserPhoneById(id);

	GetDlgItem(IDC_EDIT4)->SetWindowText(m_phone);
}
