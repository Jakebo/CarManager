// EditCarDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CarManage.h"
#include "EditCarDataDlg.h"
#include "CarOpt.h"
#include "DriverOpt.h"
#include "DeptOpt.h"
#include "TargetOpt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditCarDataDlg dialog


CEditCarDataDlg::CEditCarDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditCarDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditCarDataDlg)
	//}}AFX_DATA_INIT
}

CEditCarDataDlg::CEditCarDataDlg(BOOL addNew, CWnd *pParent /*=NULL*/)
	: CDialog(CEditCarDataDlg::IDD, pParent), addNew(addNew)
{
}

CEditCarDataDlg::CEditCarDataDlg(BOOL addNew,
								 const CStringArray &content,
								 BOOL editAble,
								 CWnd *pParent /*=NULL*/)
	: CDialog(CEditCarDataDlg::IDD, pParent), addNew(addNew), editAble(editAble)
{
	if (!addNew) { // 如果是修改或查看资料，则将要修改或查看的内容保存到 content 中
		this->content.Copy(content);
	}
}

void CEditCarDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditCarDataDlg)
	DDX_Control(pDX, IDC_COMBO5, m_targetbox);
	DDX_Control(pDX, IDC_CARTYPE, m_cartypecombox);
	DDX_Control(pDX, IDC_CARBRAND, m_carbrandcombox);
	DDX_Control(pDX, IDC_BELONGDEPT, m_belongDept);
	DDX_Control(pDX, IDC_CARSTATUS, m_carstatus);
	DDX_Control(pDX, IDC_COMBO4, m_driverbox);
	//}}AFX_DATA_MAP
}

CEditCarDataDlg::~CEditCarDataDlg()
{
}

BEGIN_MESSAGE_MAP(CEditCarDataDlg, CDialog)
	//{{AFX_MSG_MAP(CEditCarDataDlg)
	ON_BN_CLICKED(IDOK, OnSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditCarDataDlg message handlers

BOOL CEditCarDataDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	carOpt = new CCarOpt();
	CWnd *pWnd = this->GetWindow(GW_CHILD);
	InitInterface();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// InitInterface() : 根据是新增记录还是修改记录来初始用户界面
void CEditCarDataDlg::InitInterface()
{	
	if (addNew) { // 如果是新增记录
		this->SetWindowText("新增车辆资料"); // 设置对话框标题
		// 根据当前日期和当前日期已录入的记录条数生成编码，保证编码的唯一性
		COleDateTime date = COleDateTime::GetCurrentTime(); // 获取当前日期
		int	count = carOpt->GetCountByDate(date); // 获取当前日期已录入的条数
		CString countStr;

		countStr.Format("%04d", count);
		// 设置"编码"框内容
		GetDlgItem(IDC_CODING)->SetWindowText
			(date.Format("%Y%m%d") + countStr);
	} else { // 如果是修改或查看资料
		editAble ? SetWindowText("修改车辆资料") : SetWindowText("查看车辆资料");
		SetContent(content);
	}

	GetDlgItem(IDC_CODING)->EnableWindow(FALSE); // 设置"编码"框不可编辑
	InitCombox();
}

// InitCombox() : 初始化 COMBOX 控件
void CEditCarDataDlg::InitCombox()
{
	std::map<int, CString> list;
	std::map<int, CString>::iterator it, end;
	UINT nIndex;

	// 初始车辆状态的 COMBOBOX
	std::string status[6] = { "可用", "出车", "维修", "停用", "报废", "其它" };
	for (int i = 0; i < 6; ++i) {
		if ((nIndex = m_carstatus.AddString(status[i].data())) >= 0) {
			m_carstatus.SetItemData(i, i);
		}
	}

	// 初始驾驶员的 COMBOBOX
	driverOpt = new CDriverOpt();
	driverOpt->QueryDriver(list);
	m_driverbox.SetContent(list);

	// 初始部门的 COMBOBOX
	CDeptOpt deptOpt;
	list.clear();
	deptOpt.GetDept(list);
	m_belongDept.SetContent(list);

	// 初始车辆类型和车辆商标的 COMBOBOX
	CStringArray strlist;
	carOpt->GetCarType(strlist);
	m_cartypecombox.SetContent(strlist);

	strlist.RemoveAll();
	carOpt->GetCarBrand(strlist);
	m_carbrandcombox.SetContent(strlist);

	// 初始终端编码的 COMBOBOX
	CTargetOpt targetOpt;
	strlist.RemoveAll();
	targetOpt.GetUnuseCoding(strlist);
	m_targetbox.SetContent(strlist);

	if (addNew) { // 如果是新增车辆资料，设置默认的选项
		m_carstatus.SetCurSel(0);
		m_driverbox.SetCurSel(0);
	} else { // 如果是编辑车辆资料，设置车辆状态
		CString status;
		
		m_carstatus.GetWindowText(status);
		nIndex = m_carstatus.FindString(-1, status);

		m_carstatus.SetCurSel(nIndex);
	}
}

// GetContent() :  获取用户输入的所有内容
// res : 保存返回结果
void CEditCarDataDlg::GetContent(CStringArray &content)
{
	content.Copy(this->content);
}

// SetContent() : 设置窗口内容，用于修改或查看资料时
// content : 将要设置的内容
void CEditCarDataDlg::SetContent(const CStringArray &content)
{
	CWnd *pWnd = this->GetWindow(GW_CHILD);
	UINT size = content.GetSize();
	char cClassName[255] = { 0 };

	// 遍历窗口内前 size 个控件，因为 content 存储的内容
	// 有一定顺序，所以遍历控件时也要按照一定的顺序
	for (UINT i = 0; i < size; ++i) { 
		if (pWnd) {
			pWnd->EnableWindow(editAble); // 设置控件是否可编辑

			GetClassName(pWnd->GetSafeHwnd(), cClassName, 255);
			// 如果是时间控件则设置控件显示的时间
			if (strcmp(cClassName, "SysDateTimePick32") == 0) {
				COleDateTime date;
				date.ParseDateTime(content.GetAt(i));
				((CDateTimeCtrl *)pWnd)->SetTime(date);

				pWnd = pWnd->GetWindow(GW_HWNDNEXT);
				continue;
			}

			pWnd->SetWindowText(content.GetAt(i)); // 直接设置控件显示内容
			pWnd = pWnd->GetWindow(GW_HWNDNEXT); // 下一控件
		}
	}

	GetDlgItem(IDOK)->EnableWindow(editAble);
}

// OnSave() : 保存录入的记录
void CEditCarDataDlg::OnSave() 
{
	// TODO: Add your control notification handler code here
	CWnd *pWnd = this->GetWindow(GW_CHILD);
	char cClassName[255] = { 0 };
	std::string status[6] = { "可用", "出车", "维修", "停用", "报废", "其它" };
	COleDateTime date;
	CString text;
	
	// 检测关键字是否为空，看是否满足先决条件
	// 车牌号不能为空
	if (GetDlgItem(IDC_NUMBER)->GetWindowTextLength() == 0) {
		MessageBox("车牌号不能为空", "提示信息", MB_ICONWARNING);
		return;
	}
	// 车辆状态不能为空
	if (GetDlgItem(IDC_CARSTATUS)->GetWindowTextLength() == 0) {
		MessageBox("车辆状态不能为空", "提示信息", MB_ICONWARNING);
		return;
	}

	// 如果输入的终端号码，检测对应终端是否存在
	// …………

	// 检测车牌号是否重复
	GetDlgItem(IDC_NUMBER)->GetWindowText(text);
	if (addNew) {
		// 如果是新增车辆资料，直接检测是否重复
		if (carOpt->GetCountByNumber(text) > 0) {
			MessageBox("车牌号: " + text + "已经存在", "提示信息", MB_ICONWARNING);
			return;
		}
	} else {
		// 如果是修改车辆资料，先检测是否修改了车牌号
		// 如果没修改跳过检测，如果修改了再检测是否重复
		if (text.Compare(content.GetAt(1)) != 0) {
			if (carOpt->GetCountByNumber(text) > 0) {
				MessageBox("车牌号: " + text + "已经存在", "提示信息", MB_ICONWARNING);
				return;
			}
		}

		content.RemoveAll(); // 清空 CONTENT 里的所有内容
	}

	// 满足先决条件后进行保存到 CONTENT 里
	while (pWnd) {
        GetClassName(pWnd->GetSafeHwnd(), cClassName, 255);
		if (strcmp(cClassName, "Button") != 0 &&
			strcmp(cClassName, "Static") != 0) {
			if (pWnd->GetDlgCtrlID() == IDC_CARSTATUS) {
				UINT nIndex = ((CComboBox*)pWnd)->GetCurSel();
				text.Format("%d", ((CComboBox*)pWnd)->GetItemData(nIndex));
			} else {
				pWnd->GetWindowText(text);
			}
			content.Add(text);
		}

		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}

	// 根据变量 addNew 决定是新建资料还是修改资料
	if (addNew) {
		carOpt->NewCarData(content); // 将用户输入的内容新建到数据库中
	} else {
		carOpt->ModiCarData(content); // 将修改后的内容更新到数据库
	}

	int num = atoi((char*)LPCTSTR(content.GetAt(5)));
	content.SetAt(5, status[num].data());

	CDialog::OnOK();
}
