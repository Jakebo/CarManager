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

// InitInterface() : 初始化界面
void CInOutDlg::InitInterface() {
	if (m_bInOut) { // 如果是新增派车
		SetWindowText("派车"); // 设置窗口标题
		// 产生一个唯一的编码
		CTime		curDate	= CTime::GetCurrentTime();
		std::string date	= curDate.Format("%Y-%m-%d");
		int			num		= m_ioOpt.QueryRecordNumByDate(date);
		CString		coding;
		CString		numStr;

		numStr.Format("%04d", num);
		coding = curDate.Format("%Y%m%d") + numStr;

		GetDlgItem(IDC_CODING)->SetWindowText(coding);
		// 隐藏收车人编辑框
		GetDlgItem(IDC_BACKLAB)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO7)->ShowWindow(SW_HIDE);
		// 设置初始时间
		m_usectrl.SetTime(&curDate);
		m_backctrl.SetTime(&curDate);
	} else {
		SetWindowText("收车"); // 设置窗口标题
		SetContent(content); // 设置窗口内容
		m_numbercombox.EnableWindow(FALSE); // 设置车牌号不可编辑
	}

	InitCombox();
}

// InItCombox() : 初始 COMBOBOX 控件
void CInOutDlg::InitCombox()
{
	_RecordsetPtr pRst;
	std::map<int, CString> list;

	// 设置车牌号的 COMBOBOX 控件的内容
	m_carOpt.GetNumber(list);
	m_numbercombox.SetContent(list);

	// 设置驾驶员 COMBOBOX 控件的内容
	list.clear(); // 记得要清空，不然会有以前的内容
	m_driverOpt.QueryDriver(list);
	m_drivercombox.SetContent(list);

	// 设置用车人、派车人 COMBOBOX 控件的内容
	CUserOpt userOpt;
	list.clear();
	userOpt.GetUser(list);
	m_whobackcombox.SetContent(list);
	m_usercombox.SetContent(list);
	m_whooutcombox.SetContent(list);
	m_whopastcombox.SetContent(list);

	// 设置用车部门 COMBOBOX 控件内容
	CDeptOpt deptOpt;
	list.clear();
	deptOpt.GetDept(list);
	m_usedept.SetContent(list);
	
	if (!m_bInOut) { // 设置收车人 COMBOBOX 控件的内容
		m_whobackcombox.SetWindowText(CLogDlg::GetName());
	} else { // 如果是派车设置默认选项
		m_whooutcombox.SetWindowText(CLogDlg::GetName());
		m_numbercombox.SetCurSel(0);
		m_drivercombox.SetCurSel(0);
		OnSelchangeDriver();
	}

	// 设置车辆类型、品牌型号
}

void CInOutDlg::OnSelchangeNumber() 
{
	// TODO: Add your control notification handler code here
	std::string driver;
	int nIndex	= m_numbercombox.GetCurSel(); // 获取车牌号索引
	int id		= m_numbercombox.GetItemData(nIndex); // 获取车牌号对应ID

	m_carOpt.GetDriverById(id, driver); // 获取车牌号对应的驾驶员

	nIndex = m_drivercombox.FindString(-1, driver.data()); // 查找对应驾驶员索引
	
	m_drivercombox.SetCurSel(nIndex); // 选中对应的驾驶员
	OnSelchangeDriver(); // 在这里为什么不会自动触发改变选择事件?
}

void CInOutDlg::OnSelchangeDriver() 
{
	// TODO: Add your control notification handler code here
	std::string phone;
	UINT nIndex = m_drivercombox.GetCurSel(); // 获取驾驶员索引
	int  id = m_drivercombox.GetItemData(nIndex); // 获取驾驶员 ID

	m_driverOpt.QueryPhoById(id, phone); // 获取驾驶员电话
	m_driverphone = phone.data(); // 设置驾驶员电话控件的内容

	GetDlgItem(IDC_EDIT3)->SetWindowText(phone.data());; // 更新驾驶员电话控件内容
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

// NewOutCar() : 新建一个派车记录
BOOL CInOutDlg::NewOutCar()
{
	if (!DetectCondition()) {
		return FALSE;
	}

	GetTextContent();

	m_ioOpt.NewOutCarRecord(content); // 新建派车记录

	// 设置未收文本，为的是在派车记录中显示
	std::vector<std::string>::iterator it = content.end();
	CString dept;
	it -= 3;
	*it = "未收";
	// 设置部门文本
	it -= 10;
	m_usedept.GetWindowText(dept);
	*it = dept;

	return TRUE;
}

// BackCar() : 收车
BOOL CInOutDlg::BackCar()
{
	UpdateData();

	// 判断选择时间是否符合常理
	CTimeSpan	tmSp = m_usetime - m_backtime;
	if (tmSp.GetTotalSeconds() > 0) {
		MessageBox("使用时间不应大于估计返程时间", "提示信息", MB_ICONWARNING);

		return FALSE;
	}

	GetTextContent();
	m_ioOpt.InComeCar(content); // 收车

	// 按一定规矩设置好 CONTENT，为的是在派车记录/收车记录中显示
	std::vector<std::string>::iterator it = content.end();
	CString name;
	it -= 3;
	*(it++) = "已收";
	m_whobackcombox.GetWindowText(name);
	*it = name.GetBuffer(0);
	// 设置部门文本
	it -= 11; // 因为前面++了，所以这里是11
	m_usedept.GetWindowText(name);
	*it = name;

	return TRUE;
}

// DetectCondition() : 检测用户输入的内容是否满足条件
// 返回值 : 如果都满足条件，返回 TRUE，否则返回 FALSE
BOOL CInOutDlg::DetectCondition()
{
	UpdateData();

	// 判断选择时间是否符合常理
	CTimeSpan	tmSp = m_usetime - m_backtime;
	if (tmSp.GetTotalSeconds() > 0) {
		MessageBox("使用时间不应大于估计返程时间", "提示信息", MB_ICONWARNING);

		return FALSE;
	}

	// 判断选择的时间段跟已经派车的时间段是否冲突
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
		MessageBox("车牌号为: " + number + " 的车辆在时间段 " + usetime + 
				   "——" + backtime + " 内与该车辆以下派车时间均有冲突" + 
				   conflict.data(),
				   "提示信息", MB_ICONWARNING);

		return FALSE;
	}

	return TRUE;
}

// GetTextContent() : 获取窗口的文本内容
void CInOutDlg::GetTextContent()
{
	CWnd		*pWnd = GetWindow(GW_CHILD);
	char		cClassName[255] = { 0 };
	CString		text;

	// 遍历窗口控件以获取窗口内容
	content.clear();
	while (pWnd) {
		GetClassName(pWnd->GetSafeHwnd(), cClassName, 255);
		// 如果控件为 STATIC 或 BUTTON 控件，跳过
		// 否则获取控件内容
		if ((strcmp(cClassName, "Static") == 0) ||
			(strcmp(cClassName, "Button") == 0)) {
		} else {
			pWnd->GetWindowText(text);
			content.push_back(text.GetBuffer(0));
		}

		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}

	// 收尾，按一定规矩填好 CONTENT
	std::vector<std::string>::iterator it = content.end();
	it -= 2;

	CTime curTime = CTime::GetCurrentTime(); // 获取当前时间
	text = curTime.Format("%Y-%m-%d");
	content.insert(it, text.GetBuffer(0)); // 插入当前时间
	it = content.end();
	it -= 2;
	m_bInOut ? content.insert(it, "0") : // 插入是否已收车，新增派车默认为0
			   content.insert(it, "1"); // 如果是收车设置为1

	CString data;
	data.Format("%d", m_usedept.GetItemData(m_usedept.GetCurSel()));
	it -= 10;
	*it = data;
}

// GetContent() : 获取窗口内容
// 返回值 : 返回窗口的内容
const std::vector<std::string> &CInOutDlg::GetContent()
{
	return this->content;
}

// SetContent() : 设置窗口的内容
// cot : 将要设置的窗口内容
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
