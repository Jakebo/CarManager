// ManageCarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CarManage.h"
#include "ManageCarDlg.h"
#include "InOutOpt.h"
#include "InOutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CManageCarDlg dialog

CManageCarDlg::CManageCarDlg(CString &purviewCoding, CWnd* pParent /*=NULL*/)
	: CSizingDialog(CManageCarDlg::IDD, pParent), purviewCoding(purviewCoding)
{
}

void CManageCarDlg::DoDataExchange(CDataExchange* pDX)
{
	CSizingDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManageCarDlg)
	DDX_Control(pDX, IDC_ENDTIME, m_endctrl);
	DDX_Control(pDX, IDC_STARTTIME, m_startctrl);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_EDIT2, m_dept);
	DDX_DateTimeCtrl(pDX, IDC_STARTTIME, m_starttime);
	DDX_DateTimeCtrl(pDX, IDC_ENDTIME, m_endtime);
	DDX_Text(pDX, IDC_DEST, m_dest);
	DDX_Text(pDX, IDC_EDIT4, m_user);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CManageCarDlg, CSizingDialog)
	//{{AFX_MSG_MAP(CManageCarDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnSearch)
	ON_BN_CLICKED(IDC_BUTTON4, OnClear)
	ON_BN_CLICKED(IDC_RADIO2, OnInRecord)
	ON_BN_CLICKED(IDC_RADIO3, OnUnInRecord)
	ON_BN_CLICKED(IDC_RADIO1, OnOutRecord)
	ON_BN_CLICKED(IDC_BUTTON2, OnOutCar)
	ON_BN_CLICKED(IDC_BUTTON3, OnBackCar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManageCarDlg message handlers

BOOL CManageCarDlg::OnInitDialog() 
{
	CSizingDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// 以下这段代码可解决无法设置对话框无边框、大小不可变的问题
	DWORD dwStyle = GetStyle(); //获取旧样式  
    DWORD dwNewStyle = WS_OVERLAPPED | WS_VISIBLE | WS_SYSMENU | WS_CHILD | 
		WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;  
    dwNewStyle&=dwStyle; //按位与将旧样式去掉  
    SetWindowLong(this->m_hWnd, GWL_STYLE, dwNewStyle); //设置成新的样式 1073741892 
    DWORD dwExStyle = GetExStyle(); //获取旧扩展样式  
    DWORD dwNewExStyle = WS_EX_LEFT |
		WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR;
    dwNewExStyle&=dwExStyle; //按位与将旧扩展样式去掉
    SetWindowLong(this->m_hWnd, GWL_EXSTYLE, dwNewExStyle); //设置新的扩展样式  
	//告诉windows：我的样式改变了，窗口位置和大小保持原来不变！
    SetWindowPos(NULL, 0, 0, 0, 0,
		SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);

	this->SetWindowText("管理收/派车");
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT); // 设置列表选中一整行
	m_list.SetBkColor(RGB(223, 232, 238)); // 设置背景颜色

	AddCtrl(); // 添加自动适应控件
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);

	m_ioOpt = new CInOutOpt();

	// 设置两个时间控件的初始时间，如果是 DEBUG 版本
	// 默认初始时间为当前时间，但如果是 RELEASE 版本
	// 默认的初始时间却不是当前时间，这是为什么啊
	CTime curTime = CTime::GetCurrentTime();
	m_startctrl.SetTime(&curTime);
	m_endctrl.SetTime(&curTime);

	ModuleSetup(purviewCoding);
	InitList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CManageCarDlg::AddCtrl()
{
	int woc;
	HWND hwndChild = ::GetWindow(m_hWnd, GW_CHILD);

	while (hwndChild) {
		woc = ::GetDlgCtrlID(hwndChild);
		this->AddControl(woc, _T("RXY"));

		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
}

// ModuleSetup() : 根据权限设置模块的功能是否可用
void CManageCarDlg::ModuleSetup(CString &purviewCoding)
{
	// 如果编码长度为零，直接返回，不再进行功能模块设置
	// 这会产生一个问题，即当编码为空的时候，所有的都将会默认为可用的
	CWnd* pWnd = this->GetWindow(GW_CHILD);
    char cClassName[255]={0}; 
    while(pWnd)
    {   
		if (purviewCoding.GetLength() < 1) {
			return;
		}

		//得到控件的类名，主要有Edit，Button，Static等等
        GetClassName(pWnd->GetSafeHwnd(),cClassName,255);
		
		if (strcmp(cClassName, "Button") == 0) {
			if (((CButton *)pWnd)->GetButtonStyle() == 
				BS_PUSHBUTTON) {
				// 如果控件对应的编码为1，为可用，否则不可用
				if (purviewCoding.GetAt(0) == '1') {
					pWnd->EnableWindow();
				} else {
					pWnd->EnableWindow(FALSE);
				}
				purviewCoding.Delete(0); // 删除遍历过的编码
			}
		}

        pWnd = pWnd->GetWindow(GW_HWNDNEXT); // 下一控件
     }
}

// InitList() : 初始化列表
void CManageCarDlg::InitList()
{
	m_list.InsertColumn(0, _T("编码"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(1, _T("车牌号"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(2, _T("驾驶员"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(3, _T("驾驶员电话"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(4, _T("用车人"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(5, _T("用车人电话"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(6, _T("用车部门"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(7, _T("用车时间"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(8, _T("估计返程时间"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(9, _T("出发地点"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(10, _T("途经地点"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(11, _T("目的地点"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(12, _T("用车原因"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(13, _T("派车人"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(14, _T("审批人"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(15, _T("登记日期"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(16, _T("是否已收车"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(17, _T("收车人"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(18, _T("备注"), LVCFMT_LEFT, 100);

	_RecordsetPtr res;
	m_ioOpt->QueryRecord(res);

	FillList(res);
}

// FillList() : 往列表中填充内容
// res : 查询数据库得到的记录集
void CManageCarDlg::FillList(_RecordsetPtr &res)
{
	UINT row;

	m_list.DeleteAllItems();

	row = 0;
	while (!res->adoEOF)
	{
		// 设置前 7 个列的内容
		m_list.InsertItem(row, (CString)res->GetCollect("coding").bstrVal);
		for (int col = 1; col < 7; ++col) {
			if (VT_NULL != res->GetCollect(long(col)).vt) {
				CString tmp = res->GetCollect(long(col)).bstrVal;
				m_list.SetItemText(row, col, 
					(CString)res->GetCollect(long(col)).bstrVal);
			} else {
				m_list.SetItemText(row, col, "");
			}
		}

		// 设置 7，8 列的时间内容
		if (VT_NULL != res->GetCollect("usetime").vt) {
			_variant_t date = res->GetCollect("usetime");
			m_list.SetItemText(row, col++, COleDateTime(date).Format("%Y-%m-%d"));
		} else {
			m_list.SetItemText(row, col++, "");
		}

		if (VT_NULL != res->GetCollect("backtime").vt) {
			_variant_t date = res->GetCollect("backtime");
			m_list.SetItemText(row, col, COleDateTime(date).Format("%Y-%m-%d"));
		} else {
			m_list.SetItemText(row, col, "");
		}

		// 设置后面列的内容
		for (col = 9; col < 19; ++col) {
			if (col != 15) {
				if (VT_NULL != res->GetCollect(long(col)).vt) {
					m_list.SetItemText(row, col,
						(CString)res->GetCollect(long(col)).bstrVal);
				} else {
					m_list.SetItemText(row, col, "");
				}
			} else { // 如果是登记时间列，则读取时间
				if (VT_NULL != res->GetCollect("entertime").vt) {
					_variant_t date = res->GetCollect("entertime");
					m_list.SetItemText(row, col,
						COleDateTime(date).Format("%Y-%m-%d"));
				} else {
					m_list.SetItemText(row, col, "");
				}
			}
		}

		res->MoveNext();
		++row;
	}
}

// AddToLstTail() : 将一条记录添加到列表尾部
// content : 记录内容
void CManageCarDlg::AddToLstTail(std::vector<std::string> &content)
{
	std::vector<std::string>::iterator begin = content.begin();
	std::vector<std::string>::iterator end   = content.end();
	UINT row = m_list.GetItemCount();

	m_list.InsertItem(row, (*begin).data());
	++begin;

	for (int col = 1; begin != end; ++begin, ++col) {
		m_list.SetItemText(row, col, (*begin).data());
	}
}

// UpdateLstRow() : 更新某行记录的内容
// nIndex : 指定的某行
// content : 将要更新的内容
void CManageCarDlg::UpdateLstRow(const int &nIndex,
								 std::vector<std::string> &content)
{
	std::vector<std::string>::iterator begin = content.begin();
	std::vector<std::string>::iterator end   = content.end();
	begin += 2;

	for (int col = 2; begin != end; ++begin, ++col) {
		m_list.SetItemText(nIndex, col, (*begin).data());
	}
}

// GetIndexContent() : 获取某行的内容
// nIndex : 指定的一行 
// content : 保存某行的内容
void CManageCarDlg::GetIndexContent(const UINT &nIndex,
									std::vector<std::string> &content)
{
	CString text;
	CHeaderCtrl *m_pHeader = m_list.GetHeaderCtrl();
	UINT count = m_pHeader->GetItemCount();

	for (int col = 0; col < count; ++col) {
		if (col == 16 || col == 17) {
			continue;
		}
		content.push_back(m_list.GetItemText(nIndex, col).GetBuffer(0));
	}
}

void CManageCarDlg::OnSearch() 
{
	// TODO: Add your control notification handler code here
	CString searchText = "";
	unsigned char count = 0;
	CString time;
	_RecordsetPtr res;

	UpdateData();

	// 获取控件内容
	time = m_starttime.Format("%Y-%m-%d"); // 起始时间
	if (!time.IsEmpty()) {
		searchText += " usetime>='" + time + "' and";
		++count;
	}
	time = m_endtime.Format("%Y-%m-%d"); // 结束时间
	if (!time.IsEmpty()) {
		searchText += " usetime<='" + time + "' and";
		++count;
	}

	if (!m_dest.IsEmpty()) { // 目的地
		searchText += " destination like '%" + m_dest + "%' and";
		++count;
	}
	if (!m_user.IsEmpty()) { // 用车人
		searchText += " [user] like '%" + m_user + "%' and";
		++count;
	}
	if (!m_dept.IsEmpty()) { // 部门
		searchText += " dept in (select id from depts where \
						name like '%" + m_dept + "%') and";
		++count;
	}

	if (count > 0) { // 如果用户已经输入查询条件，查询
		searchText.Delete(searchText.GetLength() - 4, 4);
		m_ioOpt->QueryRecord(res, (LPCTSTR)searchText, m_radSelect); // 查询
	} else { // 如果用户没有输入任何查询条件，则刷新列表
		m_ioOpt->QueryRecord(res);
	}
	
	FillList(res);
}

void CManageCarDlg::OnClear() 
{
	// TODO: Add your control notification handler code here
	m_dest.Empty();
	m_user.Empty();
	m_dept.Empty();

	UpdateData(FALSE);
}

void CManageCarDlg::OnOutRecord() 
{
	// TODO: Add your control notification handler code here
	if (m_radSelect == 0) { // 如果不改变选择，直接返回不做处理
		return;
	}

	_RecordsetPtr res;

	m_radSelect = 0;
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);

	m_ioOpt->QueryRecord(res);
	FillList(res);
}

void CManageCarDlg::OnInRecord() 
{
	// TODO: Add your control notification handler code here
	if (m_radSelect == 1) {
		return;
	}

	_RecordsetPtr res;
	
	m_radSelect = 1;
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);

	m_ioOpt->QueryRecord(res, TRUE);
	FillList(res);
}

void CManageCarDlg::OnUnInRecord() 
{
	// TODO: Add your control notification handler code here
	if (m_radSelect == 2) {
		return;
	}

	_RecordsetPtr res;

	m_radSelect = 2;
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);

	m_ioOpt->QueryRecord(res, FALSE);
	FillList(res);
}


void CManageCarDlg::OnOutCar() 
{
	// TODO: Add your control notification handler code here
	CInOutDlg ioDlg(TRUE);

	if (ioDlg.DoModal() == IDOK) {
		std::vector<std::string> content =
			ioDlg.GetContent();

		this->AddToLstTail(content);
	}
}

void CManageCarDlg::OnBackCar() 
{
	// TODO: Add your control notification handler code here
	UINT nIndex = m_list.GetNextItem(-1, LVNI_SELECTED);
	if (nIndex == -1) {
		MessageBox("没有选中任何项", "提示信息", MB_ICONWARNING);
		return;
	}

	// 先检测该车辆是否已经收车
	CString backstatus = m_list.GetItemText(nIndex, 16);
	if (backstatus.Compare("已收") == 0) {
		MessageBox("车辆号为: " + m_list.GetItemText(nIndex, 1) +
					" 的车辆已收车", "提示信息", MB_OK);
		return;
	}	

	std::vector<std::string> content;

	GetIndexContent(nIndex, content);
	CInOutDlg ioDlg(FALSE, content);

	if (ioDlg.DoModal() == IDOK) {
		content = ioDlg.GetContent();
		UpdateLstRow(nIndex, content);
	}
}

BOOL CManageCarDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) {
		OnSearch();
		return TRUE;
	}
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)   
		return TRUE;

	return CSizingDialog::PreTranslateMessage(pMsg);
}
