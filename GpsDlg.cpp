// GpsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CarManage.h"
#include "GpsDlg.h"
#include "MapDlg.h"
#include "TraceDlg.h"
#include "PlayBackDlg.h"
#include "CarInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HWND g_hWnd;
/////////////////////////////////////////////////////////////////////////////
// CGpsDlg dialog


CGpsDlg::CGpsDlg(CString &purviewCoding, CWnd* pParent /*=NULL*/)
	: CSizingDialog(CGpsDlg::IDD, pParent), purviewCoding(purviewCoding)
{
	//{{AFX_DATA_INIT(CGpsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CGpsDlg::~CGpsDlg()
{
	std::map<int, CDialog* >::iterator it = m_traceDlg.begin();

	while (it != m_traceDlg.end()) {
		delete it->second;
		++it;
	}

	m_traceDlg.clear();

	if (carInfo != NULL) {
		delete carInfo;
	}
}

void CGpsDlg::DoDataExchange(CDataExchange* pDX)
{
	CSizingDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGpsDlg)
	DDX_Control(pDX, IDC_TREE1, m_tree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGpsDlg, CSizingDialog)
	//{{AFX_MSG_MAP(CGpsDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnTraceCar)
	ON_WM_PAINT()
	ON_MESSAGE(WM_DELETETRACEDLG, DeleteTraceDlg)
	ON_MESSAGE(WM_DELETEPBDLG, DeletePBDlg)
	ON_MESSAGE(WM_DELETECARINFODLG, DeleteCarInfoDlg)
	ON_BN_CLICKED(IDC_BUTTON2, OnPlayBack)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, OnDblclkTree1)
	ON_BN_CLICKED(IDC_BUTTON4, OnCarInfo)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnSelchangedCarTree)
	ON_MESSAGE(WM_SETMARKERVISABLE, SetMarkerVisable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGpsDlg message handlers

BOOL CGpsDlg::OnInitDialog() 
{
	CSizingDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->SetWindowText(_T("GPS功能"));
	this->ModuleSetup(purviewCoding);

	SetWindowLong(this->m_hWnd, GWL_STYLE, 1073741892);
	AddCtrl();

	this->m_tree.SetBkColor(RGB(223, 232, 238));

	targetOpt.QueryTarget(targetRecord);
	SetTreeData(targetRecord);

	g_hWnd = this->m_hWnd;
	carInfo = NULL;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// SetTreeData() : 设置树形控件的数据
// targetRecord : 要给树形控件设置的数据
void CGpsDlg::SetTreeData(std::vector<Target> &targetRecord)
{
	int nCount = targetRecord.size();
	HTREEITEM hTree; // 树根
	HTREEITEM hSubTree; // 子结点
	CStringArray args; // 调用 JS 函数时需要的参数列表
	CString targetIDStr; // ID 字符串
	CString longitudeStr; // 经度字符串
	CString latitudeStr; // 纬度字符串

	Target tmp;
	// 遍历车队
	for (int i = 0; i < nCount; ++i) { 
		tmp = targetRecord.at(i);
		hTree = m_tree.InsertItem(tmp.cargroupName, TVI_ROOT);

		int nums = tmp.targetLst.size();
		// 遍历车队里的车辆
		for (int j = 0; j < nums; ++j) { 
			TargetInfo targetInfo = tmp.targetLst.at(j);
			hSubTree = m_tree.InsertItem(targetInfo.targetName, hTree);
			m_tree.SetItemData(hSubTree, targetInfo.targetID);
		}

		m_tree.Expand(hTree, TVE_EXPAND);
	}
}

void CGpsDlg::OnTraceCar() 
{
	// TODO: Add your control notification handler code here
	CreateTraceWindow(0);
}

void CGpsDlg::OnPlayBack()
{
	// TODO: Add your control notification handler code here
	CreateTraceWindow(1);
}

// CreateTraceWindow() : 根据不同的标志创建不同的窗口
// flag : 窗口标志：0 为跟踪窗口，1 为回放窗口
void CGpsDlg::CreateTraceWindow(char flag)
{
	HTREEITEM hItem = m_tree.GetSelectedItem(); // 获取当前选中项
	if (m_tree.ItemHasChildren(hItem)) {
		return;
	}

	int targetID = m_tree.GetItemData(hItem);
	CString targetName = m_tree.GetItemText(hItem);

	CTraceDlg *traceDlg;
	CPlayBackDlg *playBackDlg;
	if (!flag) {
		std::map<int, CDialog* >::iterator it = m_traceDlg.find(targetID);
		if (it != m_traceDlg.end()) {
			return;
		}

		traceDlg = new CTraceDlg(targetID, targetName);
		traceDlg->Create(IDD_TRACEDLG, this);
		m_traceDlg[targetID] = traceDlg;
	} else {
		std::map<int, CDialog* >::iterator it = m_pBDlg.find(targetID);
		if (it != m_pBDlg.end()) {
			return;
		}

		playBackDlg = new CPlayBackDlg(targetID, targetName);
		playBackDlg->Create(IDD_PLAYBACKDLG, this);
		m_pBDlg[targetID] = playBackDlg;
	}

	SetTraceWindowPos();
}

// DeleteTraceDlg() : 删除跟踪窗口,是由跟踪窗口
//				发送消息而调用
// iParam1 : 第一个消息参数
// iParam2 : 第二个消息参数
LRESULT CGpsDlg::DeleteTraceDlg(WPARAM iParam1, LPARAM iParam2)
{
	std::map<int, CDialog* >::iterator it = m_traceDlg.find(iParam1);
	if (it != m_traceDlg.end()) {
		it->second->EndDialog(0);
		delete it->second;
		m_traceDlg.erase(it);
	}

	SetTraceWindowPos();

	return 0;
}

// DeletePBDlg() : 删除回放窗口,是由回放窗口
//				发送消息而调用
// iParam1 : 第一个消息参数
// iParam2 : 第二个消息参数
LRESULT CGpsDlg::DeletePBDlg(WPARAM iParam1, LPARAM iParam2)
{
	std::map<int, CDialog* >::iterator it = m_pBDlg.find(iParam1);
	if (it != m_pBDlg.end()) {
		it->second->EndDialog(0);
		delete it->second;
		m_pBDlg.erase(it);
	}

	SetTraceWindowPos();

	return 0;
}

// DeleteCarInfoDlg() : 删除车辆信息的窗口,是由车辆信息窗口
//					被关闭时发送消息而被调用
// wParam : 第一个消息参数
// lParam : 第二个消息参数 
LRESULT CGpsDlg::DeleteCarInfoDlg(WPARAM wParam, LPARAM lParam)
{
	if (carInfo != NULL) {
		delete carInfo;
		carInfo = NULL;
	}

	return 0;
}

// SetMarkerVisable() : 设置地图上某个标注为可见
LRESULT CGpsDlg::SetMarkerVisable(WPARAM wParam, LPARAM lParam)
{
	SetMarkerVisable(*((CString*)lParam));

	return 0;
}

// 设置跟踪窗口的位置
void CGpsDlg::SetTraceWindowPos()
{
	int row = 0; // 跟踪窗口的行数
	int col = 0; // 跟踪窗口的列数
	float div = 2; // 跟踪窗口大小跟WEB窗口大小的倍数关系
	CWebBrowser2 &web = ((CMapDlg*)this->GetParent()->GetParent())->GetWebBrowser();

	CRect rc;
	web.GetWindowRect(&rc); // 获取web的区域

	std::map<int, CDialog* >::iterator traceDlgIt = m_traceDlg.begin();
	std::map<int, CDialog* >::iterator pBDlgIt = m_pBDlg.begin();

	while (traceDlgIt != m_traceDlg.end()) {
		CRect moveRc;
		if (traceDlgIt->second != NULL) {
			moveRc.left = rc.left + (5 + ((rc.Width() / div) * col));
			moveRc.right = rc.left + ((rc.Width() / div) * (col + 1));

			// 如果已经超出WEB窗口的右边界，
			// 则行+1，列置0，并重新计算左右边界的值
			if (moveRc.right > rc.right) {
				++row;
				col = 0;

				moveRc.left = rc.left + (5 + ((rc.Width() / div) * col));
				moveRc.right = rc.left + ((rc.Width() / div) * (col + 1));
			}

			if (row == 0) { // 如果是最底一行，离bottom边20个像素
				moveRc.bottom = rc.bottom - (20 + ((rc.Height() / div) * row));
			} else { // 否则为离底下的一行5个像素
				moveRc.bottom = rc.bottom - (5 + ((rc.Height() / div) * row));
			}
			moveRc.top = rc.bottom - ((rc.Height() / div) * (row + 1));

			(traceDlgIt->second)->MoveWindow(moveRc); // 移动窗口
			(traceDlgIt->second)->ShowWindow(SW_SHOW); // 显示窗口

			++col;
			++traceDlgIt;
		}
	}

	while (pBDlgIt != m_pBDlg.end()) {
		CRect moveRc;
		if (pBDlgIt->second != NULL) {
			moveRc.left = rc.left + (5 + ((rc.Width() / div) * col));
			moveRc.right = rc.left + ((rc.Width() / div) * (col + 1));

			// 如果已经超出WEB窗口的右边界，
			// 则行+1，列置0，并重新计算左右边界的值
			if (moveRc.right > rc.right) {
				++row;
				col = 0;

				moveRc.left = rc.left + (5 + ((rc.Width() / div) * col));
				moveRc.right = rc.left + ((rc.Width() / div) * (col + 1));
			}

			if (row == 0) { // 如果是最底一行，离bottom边20个像素
				moveRc.bottom = rc.bottom - (20 + ((rc.Height() / div) * row));
			} else { // 否则为离底下的一行5个像素
				moveRc.bottom = rc.bottom - (5 + ((rc.Height() / div) * row));
			}
			moveRc.top = rc.bottom - ((rc.Height() / div) * (row + 1));

			(pBDlgIt->second)->MoveWindow(moveRc); // 移动窗口
			(pBDlgIt->second)->ShowWindow(SW_SHOW); // 显示窗口

			++col;
			++pBDlgIt;
		}
	}
}

// SetCarInfoWndPos(): 设置车辆信息窗口的位置
void CGpsDlg::SetCarInfoWndPos()
{
	CRect rc;
	CWebBrowser2 &web = ((CMapDlg*)this->GetParent()->GetParent())->GetWebBrowser();

	web.GetWindowRect(&rc); // 获取web的区域

	if (carInfo) {
		CRect tmp(rc.left + 5, (rc.bottom - rc.Width() / 6), 
							rc.right - 20, (rc.bottom - 20));
		carInfo->MoveWindow(&tmp);
		carInfo->ShowWindow(SW_SHOW);
	}
}

// CreateTraceMarker() : 创建动态跟踪或轨迹回放的车辆标志
// id : 车辆target的ID
void CGpsDlg::CreateTraceMarker(const int id)
{
	std::map<int, CDialog* >::iterator it = m_traceDlg.find(id);
	if (it != m_traceDlg.end()) {
		if (it->second) {
			((CTraceDlg*)it->second)->CreateTraceMarker();
		}
	}
}

// ModuleSetup() : 根据权限编码设置对应功能模块是否可用
// purviewCoding : 权限编码
void CGpsDlg::ModuleSetup(CString &purviewCoding)
{
	// 如果编码长度为零，直接返回，不再进行功能模块设置
	// 这会产生一个问题，即当编码为空的时候，所有的都将会默认为可用的
	CWnd* pWnd = this->GetWindow(GW_CHILD);   
    char cClassName[255]={0};
    while(pWnd)
    {
		// 如果权限编码已经用完，退出，后面的权限默认为可用
		if (purviewCoding.GetLength() < 1) {
			break;
		}

		//得到控件的类名，主要有Edit，Button，Static等等
        GetClassName(pWnd->GetSafeHwnd(),cClassName,255);
		if (strcmp(cClassName, "Button") == 0) {
			if (purviewCoding.GetAt(0) == '1') {
				pWnd->EnableWindow();
			} else {
				pWnd->EnableWindow(FALSE);
			}
			purviewCoding.Delete(0);
		}

        pWnd = pWnd->GetWindow(GW_HWNDNEXT);
     }
}

void CGpsDlg::AddCtrl()
{
	int woc;
	HWND hWndChild = ::GetWindow(m_hWnd, GW_CHILD);

	while (hWndChild) {
		woc = ::GetDlgCtrlID(hWndChild);
		this->AddControl(woc, _T("RXY"));

		hWndChild = ::GetWindow(hWndChild, GW_HWNDNEXT);
	}
}

void CGpsDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	SetTraceWindowPos();
	SetCarInfoWndPos();
	// Do not call CDialog::OnPaint() for painting messages
}

// 树形控件中的选项发生改变
void CGpsDlg::OnSelchangedCarTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (carInfo == NULL) {
		return;
	}

	HTREEITEM hItem = m_tree.GetSelectedItem();
	if (m_tree.ItemHasChildren(hItem)) { // 如果有子项，证明不是车辆结点
		return;
	}

	int targetId = m_tree.GetItemData(hItem);
	CString coding;
	targetOpt.QueryCodingById(targetId, coding);
	carInfo->SetListSel(coding);

	*pResult = 0;
}

// 双击树形控件中的ITEM
void CGpsDlg::OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	HTREEITEM hItem = m_tree.GetSelectedItem(); // 获取当前选中项
	if (m_tree.ItemHasChildren(hItem)) {
		return;
	}

	int targetId = m_tree.GetItemData(hItem);
	SetMarkerVisable(targetId);

	*pResult = 0;
}

// OnCarInfo() : 查看车辆信息事件
void CGpsDlg::OnCarInfo() 
{
	// TODO: Add your control notification handler code here
	if (carInfo == NULL) { // 如果还没有创建车辆信息窗口，则创建
		carInfo = new CCarInfo();
		carInfo->Create(IDD_CARINFO, GetDlgItem(IDD_DIALOG1));
		SetCarInfoWndPos();

		// 这里为什么要更新窗口，如果不更新，新建出来的
		// 车辆信息对话框为什么有可能不能正确显示
		this->GetParent()->Invalidate();
	}

	HTREEITEM hItem = m_tree.GetSelectedItem(); // 获取当前选中项
	if (m_tree.ItemHasChildren(hItem)) {
		return;
	}

	int targetID = m_tree.GetItemData(hItem);
	carInfo->SetListSel(targetID); // 设置信息窗口中车辆列表的选中项
}

BOOL CGpsDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)  
		return TRUE;
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)   
		return TRUE;

	return CSizingDialog::PreTranslateMessage(pMsg);
}

void CGpsDlg::ShowWindow(BOOL bShow, UINT nStatus) 
{
	CSizingDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	// 根据顶级TAB来设置动态跟踪窗口的显示与否
	std::map<int, CDialog* >::iterator begin = m_traceDlg.begin();
	std::map<int, CDialog* >::iterator end	 = m_traceDlg.end();
	UINT show = bShow ? SW_SHOW : SW_HIDE;

	while (begin != end) {
		begin->second->ShowWindow(show);
		++begin;
	}

	// 根据顶级TAB来设置轨迹回放窗口的显示与否
	begin = m_pBDlg.begin();
	end	  = m_pBDlg.end();
	
	while (begin != end) {
		begin->second->ShowWindow(show);
		++begin;
	}

	// 如果已打开了车辆信息窗口，也设置显示与否
	if (carInfo) {
		carInfo->ShowWindow(show);
	}
}

// SetMarkerVisable() : 根据终端 ID 来设置地图上的终端标注是否当前可见
// targetId : 指定的终端 ID
void CGpsDlg::SetMarkerVisable(const int &targetId)
{
	CStringArray args;
	CString tId;

	tId.Format("%d", targetId);
	args.Add(tId);

	targetOpt.QueryTargetInfo(targetId, args);

	((CMapDlg*)this->GetParent()->GetParent())->CallJSFunc("getMarkerByID",
															args);
}

// SetMarkerVisable() : 根据终端编码来设置地图上的终端标注是否当前可见
// targetCoding : 指定的终端编码
void CGpsDlg::SetMarkerVisable(const CString &targetCoding)
{
	int id = targetOpt.QueryIdByCoding(targetCoding);
	
	SetMarkerVisable(id);
}
