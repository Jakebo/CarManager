// TMDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CarManage.h"
#include "TMDlg.h"
#include "EditTargetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTMDlg dialog

CTMDlg::CTMDlg(CString &purviewCoding, CWnd *pParent/*=NULL*/)
	:CSizingDialog(CTMDlg::IDD, pParent), m_purview(purviewCoding)
{
}

void CTMDlg::DoDataExchange(CDataExchange* pDX)
{
	CSizingDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTMDlg)
	DDX_Control(pDX, IDC_TARGETLST, m_list);
	DDX_Text(pDX, IDC_EDIT1, m_searchText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTMDlg, CSizingDialog)
	//{{AFX_MSG_MAP(CTMDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnNewTarget)
	ON_BN_CLICKED(IDC_BUTTON2, OnModi)
	ON_BN_CLICKED(IDC_BUTTON3, OnDel)
	ON_BN_CLICKED(IDC_BUTTON4, OnSearch)
	ON_BN_CLICKED(IDC_BUTTON5, OnUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTMDlg message handlers
// FillLst() : 填充列表内容

BOOL CTMDlg::OnInitDialog() 
{
	CSizingDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_list.SetBkColor(RGB(223, 232, 238)); // 设置背景颜色
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT); // 设置列表选中一整行
	
	ModuleSetup(m_purview);
	InitInterface();
	FillLst();
	AddCtrl();

	// 设置控件显示的TOOLTIP
	EnableToolTips(TRUE);
	m_toolTip.Create(this);
	m_toolTip.Activate(TRUE);
	m_toolTip.AddTool(GetDlgItem(IDC_EDIT1),
		"搜索关键字:\'终端名\',\'当前地址\',\'所属车队\',\'所属用户组\'");


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// ModuleSetup() : 根据权限设置模块的功能是否可用
void CTMDlg::ModuleSetup(CString &purviewCoding)
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

// AddCtrl() : 将控件添加到自适应大小中
void CTMDlg::AddCtrl() 
{
	int  woc;
	CWnd *pWnd = GetWindow(GW_CHILD);

	while (pWnd) {
		woc = pWnd->GetDlgCtrlID();
		this->AddControl(woc);
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}
}

// 初始界面
void CTMDlg::InitInterface()
{
	this->SetWindowText("终端管理");

	m_list.InsertColumn(0, _T("终端编码"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(1, _T("终端名"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(2, _T("当前地址"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(3, _T("证书"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(4, _T("终端电话"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(5, _T("权限"), LVCFMT_LEFT, 40);
	m_list.InsertColumn(6, _T("是否在线"), LVCFMT_LEFT, 60);
	m_list.InsertColumn(7, _T("所属车队"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(8, _T("用户组"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(9, _T("备注"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(10, _T("登记时间"), LVCFMT_LEFT, 80);
}

// FillLst() : 填充列表
void CTMDlg::FillLst()
{
	_RecordsetPtr pRst;

	targetOpt.QueryTargetInfo(pRst);
	m_list.SetListContent(pRst);
}

void CTMDlg::OnNewTarget() 
{
	// TODO: Add your control notification handler code here
	CEditTargetDlg etDlg;

	if (etDlg.DoModal() == IDOK) {
		CStringArray &content = etDlg.GetContent();

		AddNewRow(content);
	}
}

void CTMDlg::OnModi() 
{
	// TODO: Add your control notification handler code here
	CStringArray content;
	int			 row = m_list.GetNextItem(-1, LVNI_SELECTED);

	GetRowContent(content);
	CEditTargetDlg etDlg(content);
	if (etDlg.DoModal() == IDOK) {
		CStringArray &modi = etDlg.GetContent();
		SetRowContent(row, modi);
	}
}

void CTMDlg::OnDel() 
{
	// TODO: Add your control notification handler code here
	int		nIndex = m_list.GetNextItem(-1, LVNI_SELECTED);
	CString coding = m_list.GetItemText(nIndex, 0);

	targetOpt.DelTarget(coding);
	m_list.DeleteItem(nIndex);
}

// AddNewRow() : 在列表中添加一行
// content : 新增一行的内容
void CTMDlg::AddNewRow(const CStringArray &content)
{
	int row  = m_list.GetItemCount();

	m_list.InsertItem(row, ""); // 新增一行

	SetRowContent(row, content);
}

// GetRowContent() : 按照一定要求获取选中的某行的内容
// content : 保存并返回得到的内容
void CTMDlg::GetRowContent(CStringArray &content)
{
	// 先获取某行所有的内容
	m_list.GetCurSelContent(content);

	// 按一定要求对得到的内容进行处理
	content.RemoveAt(2);
	content.RemoveAt(5);
	content.RemoveAt(8);
}

// SetRowContent() : 设置某行的内容
// content : 将要设置的内容
void CTMDlg::SetRowContent(const int row, const CStringArray &content)
{
	int size = content.GetSize();
	int incr = 0;

	for (int index = 0; index < size; ++index) {
		switch (index) {
		case 2:
			++incr;
			m_list.SetItemText(row, index + 1, content.GetAt(index));
			break;
		case 5:
			++incr;
			m_list.SetItemText(row, index + 2, content.GetAt(index));
			break;
		default:
			m_list.SetItemText(row, index + incr, content.GetAt(index));
			break;
		}
	}
}

BOOL CTMDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_toolTip.RelayEvent(pMsg);

	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) {
		OnSearch();
		return TRUE;
	}
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)   
		return TRUE;

	return CSizingDialog::PreTranslateMessage(pMsg);
}

void CTMDlg::OnSearch() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if (m_searchText.GetLength() > 0) {
		_RecordsetPtr res;
		targetOpt.SearchByText(m_searchText, res);
		m_list.SetListContent(res);
	} else {
		FillLst();
	}
}

void CTMDlg::OnUpdate() 
{
	// TODO: Add your control notification handler code here
	FillLst();
}
