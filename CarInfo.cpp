// CarInfo.cpp : implementation file
//

#include "stdafx.h"
#include "CarManage.h"
#include "CarInfo.h"
#include "CarOpt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HWND g_hWnd;
/////////////////////////////////////////////////////////////////////////////
// CCarInfo dialog
extern HWND g_hWnd;

CCarInfo::CCarInfo(CWnd* pParent /*=NULL*/)
	: CSizingDialog(CCarInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCarInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CCarInfo::DoDataExchange(CDataExchange* pDX)
{
	CSizingDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCarInfo)
	DDX_Control(pDX, IDC_LIST2, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCarInfo, CSizingDialog)
	//{{AFX_MSG_MAP(CCarInfo)
	ON_BN_CLICKED(IDC_BUTTON3, OnClose)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, OnDblclkInfoList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCarInfo message handlers

BOOL CCarInfo::OnInitDialog() 
{
	CSizingDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	// 以下这段代码可解决无法设置对话框无边框、大小不可变的问题
	DWORD dwStyle = GetStyle(); //获取旧样式  
    DWORD dwNewStyle = WS_OVERLAPPED | WS_VISIBLE | WS_SYSMENU | WS_CHILD | 
		WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;  
    dwNewStyle&=dwStyle; //按位与将旧样式去掉  
    SetWindowLong(this->m_hWnd, GWL_STYLE, dwNewStyle); //设置成新的样式  
    DWORD dwExStyle = GetExStyle(); //获取旧扩展样式  
    DWORD dwNewExStyle = WS_EX_LEFT |
		WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR;  
    dwNewExStyle&=dwExStyle; //按位与将旧扩展样式去掉  
    SetWindowLong(this->m_hWnd, GWL_EXSTYLE, dwNewExStyle); //设置新的扩展样式  
	//告诉windows：我的样式改变了，窗口位置和大小保持原来不变！
    SetWindowPos(NULL, 0, 0, 0, 0,
		SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);

	AddCtrl(); // 将窗口的所有控件都添加到自适应窗口大小

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT); // 设置列表选中一整行
	m_list.SetBkColor(RGB(223, 232, 238));

	m_list.InsertColumn(0, "编码", LVCFMT_LEFT, 80);
	m_list.InsertColumn(1, "车牌", LVCFMT_LEFT, 100);
	m_list.InsertColumn(2, "品牌型号", LVCFMT_LEFT, 80);
	m_list.InsertColumn(3, "车辆类型", LVCFMT_LEFT, 80);
	m_list.InsertColumn(4, "所属部门", LVCFMT_LEFT, 100);
	m_list.InsertColumn(5, "车辆状态", LVCFMT_LEFT, 100);
	m_list.InsertColumn(6, "驾驶员", LVCFMT_LEFT, 100);
	m_list.InsertColumn(7, "购买日期", LVCFMT_LEFT, 100);
	m_list.InsertColumn(8, "登记日期", LVCFMT_LEFT, 100);
	m_list.InsertColumn(9, "终端号码", LVCFMT_LEFT, 100);

	_RecordsetPtr pRst;
	CCarOpt carOpt;
	carOpt.QueryAllCarInfo(pRst);
	FillingInfo(pRst);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// SetTargetId() : 设置列表框中的当前选中项为指定的项
// targetId : 将要指定的target ID
void CCarInfo::SetListSel(const CString &coding)
{
	int nCount = m_list.GetItemCount(); // 获取列表框中的条目数量
	CString text;
	
	if (nCount <= 0) {
		return;
	}

	// 遍历列表寻找跟参数一致的项
	for (int i = 0; i < nCount; ++i) {
		text = m_list.GetItemText(i, 9);  // 获取ITEM TEXT
		if (coding == text) { // 匹配数据
			// 设置对应的项为选中状态
			m_list.EnsureVisible(i, FALSE); // 确保列表中选中的行可见
			m_list.SetFocus(); // 设置列表框获取焦点
			m_list.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED); // 设置列表框中选中的项
			break;
		}
	}
}

// FillingInfo() : 填充控件的信息内容
void CCarInfo::FillingInfo(_RecordsetPtr &pRst)
{
	CString tmpStr;
	UINT row, col;

	m_list.DeleteAllItems();
	row = 0;
	while (!pRst->adoEOF) {
		col = 0;

		if (VT_NULL != pRst->GetCollect("coding").vt) { // 判断字段值是否为空
			tmpStr = pRst->GetCollect("coding").bstrVal;
		}
		m_list.InsertItem(row, tmpStr);
		tmpStr = "";
		m_list.SetItemData(row * 6, pRst->GetCollect("id").lVal);
		++col;

		if (VT_NULL != pRst->GetCollect("number").vt) {
			tmpStr = pRst->GetCollect("number").bstrVal;
		}
		m_list.SetItemText(row, col, tmpStr);
		tmpStr = "";
		++col;

		if (VT_NULL != pRst->GetCollect("carbrand").vt) {
			tmpStr = pRst->GetCollect("carbrand").bstrVal;
		}
		m_list.SetItemText(row, col, tmpStr);
		tmpStr = "";
		++col;

		if (VT_NULL != pRst->GetCollect("cartype").vt) {
			tmpStr = pRst->GetCollect("cartype").bstrVal;
		}
		m_list.SetItemText(row, col, tmpStr);
		tmpStr = "";
		++col;

		if (VT_NULL != pRst->GetCollect("belongdept").vt) {
			tmpStr = pRst->GetCollect("belongdept").bstrVal;
		}
		m_list.SetItemText(row, col, tmpStr);
		tmpStr = "";
		++col;

		if (VT_NULL != pRst->GetCollect("status").vt) {
			tmpStr = pRst->GetCollect("status").bstrVal;
		}
		m_list.SetItemText(row, col, tmpStr);
		tmpStr = "";
		++col;

		if (VT_NULL != pRst->GetCollect("driver").vt) {
			tmpStr = pRst->GetCollect("driver").bstrVal;
		}
		m_list.SetItemText(row, col, tmpStr);
		tmpStr = "";
		++col;

		if (VT_NULL != pRst->GetCollect("buytime").vt) {
			_variant_t date = pRst->GetCollect("buytime");
			tmpStr = COleDateTime(date).Format("%Y-%m-%d");
		}
		m_list.SetItemText(row, col, tmpStr);
		tmpStr = "";
		++col;

		if (VT_NULL != pRst->GetCollect("entertime").vt) {
			_variant_t date = pRst->GetCollect("entertime");
			tmpStr = COleDateTime(date).Format("%Y-%m-%d");	
		}
		m_list.SetItemText(row, col, tmpStr);
		tmpStr = "";
		++col;

		if (VT_NULL != pRst->GetCollect("terminalnum").vt) {
			tmpStr = pRst->GetCollect("terminalnum").bstrVal;
		}
		m_list.SetItemText(row, col, tmpStr);
		tmpStr = "";

		pRst->MoveNext();
		++row;
	}

	m_list.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED); // 默认选中第一行
}

// AddCtrl() : 将该窗口的所有控件全部添加到自适应窗口大小的控件中
void CCarInfo::AddCtrl()
{
	int woc;
	HWND hwndChild = ::GetWindow(m_hWnd, GW_CHILD);

	while (hwndChild) {
		woc = ::GetDlgCtrlID(hwndChild);
		this->AddControl(woc, _T("RXY"));

		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
}

BOOL CCarInfo::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)  
		return TRUE;
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) {
		// 关闭窗口
		::SendMessage(g_hWnd, WM_DELETECARINFODLG, 0, 0);
		return TRUE;
	}

	return CSizingDialog::PreTranslateMessage(pMsg);
}

void CCarInfo::OnClose()
{
	// TODO: Add your control notification handler code here
	::SendMessage(g_hWnd, WM_DELETECARINFODLG, 0, 0);
}

void CCarInfo::OnDblclkInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int		nIndex = m_list.GetNextItem(-1, LVNI_SELECTED);
	CString coding = m_list.GetItemText(nIndex, 9);

	if (coding.GetLength() < 1) {
		return;
	}

	::SendMessage(g_hWnd, WM_SETMARKERVISABLE, 0, (LPARAM)&coding);

	*pResult = 0;
}
