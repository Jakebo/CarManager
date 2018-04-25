// RoleManageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CarManage.h"
#include "RoleManageDlg.h"
#include "LogDlg.h"
#include "NewRoleDlg.h"
#include "MyTabCtrl.h"
#include "NewUserDlg.h"

#include <windowsx.h>

#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRoleManageDlg dialog

// 引用成员要在初始化列表里初始化
CRoleManageDlg::CRoleManageDlg(CString &purviewCoding,
							   CWnd *pParent /*=NULL*/)
	: CDialog(CRoleManageDlg::IDD, pParent), purviewCoding(purviewCoding)
{
	//{{AFX_DATA_INIT(CRoleManageDlg)
	//}}AFX_DATA_INIT
	
}


void CRoleManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRoleManageDlg)
	DDX_Control(pDX, IDC_LIST1, m_userLst);
	DDX_Control(pDX, IDC_TREE1, m_perThree);
	DDX_Control(pDX, IDC_LIST2, m_roleLst);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRoleManageDlg, CDialog)
	//{{AFX_MSG_MAP(CRoleManageDlg)
	ON_WM_SIZE()
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeUserLst)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, OnItemchangedRoleLst)
	ON_BN_CLICKED(IDC_BUTTON3, OnSavePermission)
	ON_BN_CLICKED(IDC_BUTTON5, OnNewRole)
	ON_BN_CLICKED(IDC_BUTTON6, OnDelRole)
	ON_NOTIFY(NM_CLICK, IDC_TREE1, OnClickTree1)
	ON_NOTIFY(TVN_KEYDOWN, IDC_TREE1, OnKeydownTree1)
	ON_BN_CLICKED(IDC_BUTTON8, OnDelUser)
	ON_BN_CLICKED(IDC_BUTTON7, OnNewUser)
	ON_BN_CLICKED(IDC_BUTTON4, OnModiInfo)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRoleManageDlg message handlers

BOOL CRoleManageDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->SetWindowText(_T("权限管理"));

	// 权限权限设置各模块功能是否可用
	ModuleSetup(purviewCoding);

	// 设置角色列表的风格为有CHECKBOX
	int style = m_roleLst.GetExtendedStyle();
	style |= (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES |
			 LVS_EX_INFOTIP | LVS_EX_CHECKBOXES);
	m_roleLst.SetExtendedStyle(style | LVS_EX_FULLROWSELECT);
	
	m_roleLst.SetBkColor(RGB(223, 232, 238));
	m_perThree.SetBkColor(RGB(223, 232, 238));

	ro.QueryUserByName(userMap);
	ro.QueryRole(roleMap);

	this->SetUserLstData(userMap);
	this->SetRoleLstData(roleMap);

	// Get the Client rect for resize use
	CRect rect;
	GetClientRect(&rect);
	old.x = rect.right - rect.left;
	old.y = rect.bottom - rect.top;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRoleManageDlg::SetUserLstData(std::map<int, CString> &userMap)
{
	std::map<int, CString>::iterator it = userMap.begin();

	int len = userMap.size();

	for (int i = 0; i < len; ++i) {
		m_userLst.InsertString(i, it->second); // It can't use AddString
		m_userLst.SetItemData(i, it->first);
		++it;
	}
}

// SetRoleLstData() : 设置角色列表的数据
// roleMap : 将要设置的角色列表的MAP数据
void CRoleManageDlg::SetRoleLstData(std::map<int, CString> &roleMap)
{
	std::map<int, CString>::iterator it = roleMap.begin();
	int len = roleMap.size();

	for (int i = 0; i < len; ++i) {
		m_roleLst.InsertItem(i, it->second);
		m_roleLst.SetItemData(i, it->first);
		++it;
	}
}

// Set tree's item data
void CRoleManageDlg::SetPerTreeData()
{
	// 获取顶级的TAB控件
	CMyTabCtrl *pTab = (CMyTabCtrl *)this->GetParent();
	// 取得顶级TAB控件管理的所有窗口
	std::map<int, CDialog *> dlgMap = pTab->GetDlgMap();
	std::map<int, CDialog *>::iterator it = dlgMap.begin();
	HTREEITEM hTree;

	while (it != dlgMap.end()) {
		// 遍历窗口取得与权限有关的控件，并在权限树中显示出来
		GetWndChild(FALSE, hTree, it->second);

		++it;
	}
}

// GetWndChild() : 遍历窗口的控件，同时将相应的项添加到权限树中
// parentItem : 递归中的树的父结点
// pWnd : 将要遍历的窗口指针
void CRoleManageDlg::GetWndChild(BOOL haveParent,
								 HTREEITEM &parentItem,
								 CWnd *pWnd)
{
	if (!pWnd) {
		return;
	}

	CWnd *childWnd = pWnd->GetWindow(GW_CHILD); // 获取窗口的子控件
	char cClassName[255] = { 0 };
	HTREEITEM hTree;
	HTREEITEM childItem;
	CString text;

	pWnd->GetWindowText(text);
	if (haveParent) { // 有父窗口，为子项
		hTree = m_perThree.InsertItem(text, parentItem);
	} else { // 否则为根
		hTree = m_perThree.InsertItem(text, TVI_ROOT);
	}
	// 设置树中项的数据
	m_perThree.SetItemData(hTree,
		theApp.topDlgLst[pWnd->GetDlgCtrlID()]);

	while (childWnd) {
		// 获取控件的类名
		GetClassName(childWnd->GetSafeHwnd(), cClassName, 255);
		// 如果类名为Button，添加到树中并设置相应项的数据
		// 如果类名为systabcontrol32，则继续遍历该TAB控件所管理的窗口
		if (strcmp(cClassName, "Button") == 0) {
			// 判断是否是 PUSHBUTTON,因为 RADIOBUTTON 和 CHECKBUTTON
			// 的类名也是 CButton
			if (((CButton *)childWnd)->GetButtonStyle() == BS_PUSHBUTTON) {
				childWnd->GetWindowText(text); // 获取按钮上的文本

				childItem = m_perThree.InsertItem(text, hTree);
				m_perThree.SetItemData(childItem, childWnd->GetDlgCtrlID());
			}
		} else if (strcmp(cClassName, "SysTabControl32") == 0) {
			std::map<int, CDialog *> dlgMap =
				((CMyTabCtrl *)childWnd)->GetDlgMap();
			std::map<int, CDialog *>::iterator it = dlgMap.begin();

			while (it != dlgMap.end()) {
				GetWndChild(TRUE, hTree, it->second);
				++it;
			}
		}

		childWnd = childWnd->GetWindow(GW_HWNDNEXT);
	}
	m_perThree.Expand(hTree, TVE_EXPAND); // 展开树
}

// SetRoleLstCheck() : 根据列表中的选中的管理员所
//			属的角色来设置角色列表的中Check状态
// userRoleMap : 列表中选中管理员所属的角色
// 实现方法：遍历传进来的userRoleMap参数，并从RoleLst中查找对应的匹配项
//		如果存在匹配项，则勾选该匹配项，如果没有，则忽略
void CRoleManageDlg::SetRoleLstCheck(std::map<int, CString> &userRoleMap)
{
	LVFINDINFO info;
	int nIndex;
	int len = userRoleMap.size();
	std::map<int, CString>::iterator it = userRoleMap.begin();

	info.flags = LVFI_STRING; // Fine entire match
	
	for (int i = 0; i < len; ++i) {
		info.psz = it->second;
		nIndex = m_roleLst.FindItem(&info); // 查找对应的项
		if (nIndex != -1) {
			m_roleLst.SetCheck(nIndex, TRUE);
		}
		++it;
	}
}

// Set role list item status
void CRoleManageDlg::SetRoleLstNoCheck()
{
	int nCount = m_roleLst.GetItemCount();

	for (int i = 0; i < nCount; ++i) {
		m_roleLst.SetCheck(i, FALSE);
	}
}

// SetPerTreeCheck() : 根据权限编码来设置权限树的选中状态
// hTree : 将要遍历的树结点
// purviewCoding : 权限编码
void CRoleManageDlg::SetPerTreeCheck(HTREEITEM &hTree,
									 CString &purviewCoding)
{
	HTREEITEM hasChildItem;
	int nCount = 0;

	// 如果提供的树结点不为空，则遍历该结点及他的兄弟结点
	while (hTree) {
		if (m_perThree.ItemHasChildren(hTree)) { // 如果遇到某结点有子结点(表示遇到TAB)
			if (nCount == 0) {
				hasChildItem = hTree; // 保存第一个遇到的有子结点的项
			}
			++nCount;
		}

		// 如果已经没有编码，返回
		if (purviewCoding.GetLength() < 1) {
			return;
		}
		// 如果该结点对应的编码为1，设置为选中状态
		if (purviewCoding.GetAt(0) == '1') {
			m_perThree.SetCheck(hTree, TRUE);
		}

		purviewCoding.Delete(0); // 删除一个编码
		hTree = m_perThree.GetNextSiblingItem(hTree); // 获取下一个兄弟结点
	}

	// 如果存在有子结点的项，则遍历所有存在有子结点的项
	if (nCount != 0) {
		// 如果遍历的TAB管理窗口的个数不足一个单元，则补够一个单元
		purviewCoding.Delete(0, theApp.unit - nCount);
		HTREEITEM childItem;
		do {
			childItem = m_perThree.GetChildItem(hasChildItem);
			// 遍历子结点
			SetPerTreeCheck(childItem, purviewCoding);
			// 获取下一兄弟结点
			hasChildItem = m_perThree.GetNextSiblingItem(hasChildItem); 
		} while (hasChildItem);
	}
}

// Set tree's item status no check
void CRoleManageDlg::SetPerTreeNoCheck()
{
	HTREEITEM root = m_perThree.GetRootItem(); // 获取权限树的根

	while (root != NULL) {
		RecursionTree(root); // 递归结点
		root = m_perThree.GetNextSiblingItem(root);
	} 
}

// GetPerTreeCheck() : 获取权限树被选中的项，生成权限编码
void CRoleManageDlg::GetPerTreeCheck(CString &purviewCoding)
{
	purviewCoding = _T("");
	HTREEITEM root = m_perThree.GetRootItem();

	RecursionForGetCheck(root, purviewCoding);
}

// RecursionForGetCheck() : 获取权限树的选中状态，方法跟设置权限树的
//					选中状态差不多
// hTree :  将要遍历获取的树结点
// purviewCoding : 保存获取到的编码
void CRoleManageDlg::RecursionForGetCheck(HTREEITEM &hTree,
										  CString &purviewCoding)
{
	if (!hTree) { // 如果参数为空，返回
		return;
	}

	HTREEITEM hasChildItem;
	int nCount = 0;
	while (hTree != NULL) {
		if (m_perThree.ItemHasChildren(hTree)) {
			if (nCount == 0) {
				hasChildItem = hTree;
			}
			++nCount;
		}
		// 如果结点为选中状态，则对应编码为1，否则为0
		if (m_perThree.GetCheck(hTree)) {
			purviewCoding += "1";
		} else {
			purviewCoding += "0";
		}
		hTree = m_perThree.GetNextSiblingItem(hTree);
	}

	if (nCount != 0) {
		// 如果不够一个TAB管理窗口的单元，则补足一个单元
		for (int i = (theApp.unit - nCount); i > 0; --i) {
			purviewCoding += "0";
		}

		HTREEITEM childItem; // 保存子项
		do {
			childItem = m_perThree.GetChildItem(hasChildItem);
			RecursionForGetCheck(childItem, purviewCoding);
			hasChildItem = m_perThree.GetNextSiblingItem(hasChildItem);
		} while (hasChildItem);
	}
}

// ModuleSetup() : 根据权限设置模块的功能是否可用
void CRoleManageDlg::ModuleSetup(CString &purviewCoding)
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
			// 如果控件对应的编码为1，为可用，否则不可用
			if (purviewCoding.GetAt(0) == '1') {
				pWnd->EnableWindow();
			} else {
				pWnd->EnableWindow(FALSE);
			}
			purviewCoding.Delete(0); // 删除遍历过的编码
		}

        pWnd = pWnd->GetWindow(GW_HWNDNEXT); // 下一控件
     }
}

// RecursionTree() : 使用递归来遍历参数所提供的根结点,并设置遍历
//					到的项的选择状态为否(不选中)
// hRoot : 将要遍历的根结点
void CRoleManageDlg::RecursionTree(HTREEITEM &hRoot)
{
	m_perThree.SetCheck(hRoot, FALSE); // 设置结点的选中状态为否

	if (m_perThree.ItemHasChildren(hRoot)) {
		HTREEITEM hChildItem = m_perThree.GetChildItem(hRoot);
		while (hChildItem != NULL) {
			RecursionTree(hChildItem);
			hChildItem = m_perThree.GetNextItem(hChildItem, TVGN_NEXT);
		}
	}
}

void CRoleManageDlg::Resize()
{
	float fsp[2];
	POINT Newp; //获取现在对话框的大小
	CRect recta;

	GetClientRect(&recta);     //取客户区大小
	Newp.x=recta.right-recta.left;
	Newp.y=recta.bottom-recta.top;
	fsp[0]=(float)Newp.x/old.x;
	fsp[1]=(float)Newp.y/old.y;

	CRect Rect;
	int woc;
	CPoint OldTLPoint,TLPoint; //左上角
	CPoint OldBRPoint,BRPoint; //右下角

	HWND  hwndChild=::GetWindow(m_hWnd,GW_CHILD);  //列出所有控件
	while(hwndChild)  
	{  
		woc=::GetDlgCtrlID(hwndChild);//取得ID
		GetDlgItem(woc)->GetWindowRect(Rect);
		ScreenToClient(Rect);
		OldTLPoint = Rect.TopLeft();
		TLPoint.x = long(OldTLPoint.x*fsp[0]);
		TLPoint.y = long(OldTLPoint.y*fsp[1]);
		OldBRPoint = Rect.BottomRight();
		BRPoint.x = long(OldBRPoint.x *fsp[0]);
		BRPoint.y = long(OldBRPoint.y *fsp[1]);
		Rect.SetRect(TLPoint,BRPoint);
		GetDlgItem(woc)->MoveWindow(Rect,TRUE);
		hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);  
	}

	old=Newp;
}

void CRoleManageDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	Resize();
}

void CRoleManageDlg::OnSelchangeUserLst() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_userLst.GetCurSel();
	CString userName;
	int userID;
	
	userID = m_userLst.GetItemData(nIndex);

	userRoleMap.clear(); // Clear the role map
	ro.QueryRoleByUserID(userID, userRoleMap); // Query master's role & store in userRoleMap

	SetRoleLstNoCheck(); // Don't check any list item
	SetRoleLstCheck(userRoleMap); // Update list item status
}

void CRoleManageDlg::OnItemchangedRoleLst(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int nIndex = m_roleLst.GetNextItem(-1, LVNI_SELECTED);
	if (nIndex != -1) {
		int roleID = m_roleLst.GetItemData(nIndex); // 获取角色ID

		CString purviewCoding;
		ro.QueryPermissByRID(roleID, purviewCoding); // 查询相应角色的权限编码

		HTREEITEM root = m_perThree.GetRootItem();
		
		SetPerTreeNoCheck();
		if (purviewCoding.GetLength() == 0) { // 如果权限编码为空，权限树全不选
			return;
		}

		SetPerTreeCheck(root, purviewCoding); // 设置权限树的选中状态
	}

	*pResult = 0;
}

// OnSavePermission() : 点击"保存权限"按钮事件
//				对用户做出的权限修改进行保存
void CRoleManageDlg::OnSavePermission() 
{
	// TODO: Add your control notification handler code here
	CString purviewCoding;

	SaveMRRelation(); // 保存用户-角色的关系
	this->GetPerTreeCheck(purviewCoding); // 获取权限树中的选中状态
	SaveRolePurviewCoding(purviewCoding); // 保存角色-权限编码关系
}

// SaveMRrelation() : 保存改变之后的管理员-角色关系
void CRoleManageDlg::SaveMRRelation()
{
	std::map<int, CString> addMR; // Store the add master-role records
	std::map<int, CString> delMR; // Store the delete master-role records
	int mId; // Master id
	int cmId = CLogDlg::GetID(); // Get the create master id

	int nIndex = m_userLst.GetCurSel(); // Get the current select item's data
	if (nIndex != -1) {
		GetDiffRoleAtRoleLst(m_roleLst,
			addMR,
			delMR,
			userRoleMap);

		mId = m_userLst.GetItemData(nIndex); // Get the selected master id
		CTime now = CTime::GetCurrentTime(); // Get current time

		if (addMR.size() > 0) {
			ro.AddRoleIntoMasterRole(mId,
				cmId,
				now.Format("%y-%m-%d   %H:%M:%S"),
				addMR);
		}

		if (delMR.size() > 0) {
			ro.DelRoleFromMasterRole(mId,
				delMR);
		}
	}
}

// SaveRolePurviewCoding() : 保存改变之后的角色对应的权限编码
void CRoleManageDlg::SaveRolePurviewCoding(CString &purviewCoding)
{
	int roleID;
	int nIndex = m_roleLst.GetNextItem(-1, LVNI_SELECTED);

	if (nIndex != -1) {
		roleID = m_roleLst.GetItemData(nIndex);
		CTime now = CTime::GetCurrentTime();

		ro.SaveRPC(roleID,
				   CLogDlg::GetID(),
				   now.Format("%y-%m-%d %H:%M:%S"),
				   purviewCoding);
	}
}


// GetDiffRoleAtRoleLst() : 根据通过角色列表所选择的项，来判断某
//		管理员修改后的角色跟原有的角色的不同，以决定角色的增删
// roleLst : 角色列表
// addMR : 返回需要为某个管理员新增角色的ID
// delMR : 返回需要为某个管理员删除角色的ID
// URMap : 某管理员原有角色的ID
void CRoleManageDlg::GetDiffRoleAtRoleLst(CListCtrl &roleLst,
										  std::map<int, CString> &addMR,
										  std::map<int, CString> &delMR,
										  std::map<int, CString> &URMap)
{
	std::map<int, CString>::iterator it;
	int nCount = roleLst.GetItemCount();

	for (int i = 0; i < nCount; ++i) {
		if (roleLst.GetCheck(i)) {
			it = URMap.find(roleLst.GetItemData(i));
			if (it == URMap.end()) { // userRoleMap haven't role, but checked, Add it
				addMR[roleLst.GetItemData(i)] =
					roleLst.GetItemText(i, 0);
			}
		} else {
			it = URMap.find(roleLst.GetItemData(i));
			if (it != URMap.end()) { // userRoleMap have role, but not checked, Delete it
				delMR[roleLst.GetItemData(i)] =
					roleLst.GetItemText(i, 0);
			}
		}
	}
}

// OnNewRole() : 新增角色
void CRoleManageDlg::OnNewRole() 
{
	// TODO: Add your control notification handler code here
	CNewRoleDlg nrDlg;
	if (nrDlg.DoModal() == IDOK) {
		ro.QueryRole(roleMap);

		m_roleLst.DeleteAllItems();
		this->SetRoleLstData(roleMap);
		this->OnSelchangeUserLst();
	}
}

// OnDelRole() : 删除列表中选定角色
void CRoleManageDlg::OnDelRole() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_roleLst.GetNextItem(-1, LVNI_SELECTED);

	if (nIndex == -1) {
		MessageBox("你没有选择任何角色", "提示信息", MB_ICONWARNING);
		return;
	}

	if (AfxMessageBox("是否删除角色 " + m_roleLst.GetItemText(nIndex, 0) + " ?",
		MB_OK | MB_OKCANCEL) == IDCANCEL) {
		return;
	}

	int rID = m_roleLst.GetItemData(nIndex); // 获取选中项的ID
	if (ro.DelRole(rID)) { // 删除选中的项
		roleMap.clear(); // 清除角色查询记录
		ro.QueryRole(roleMap); // 重新查询删除角色后的记录

		m_roleLst.DeleteAllItems(); // 删除角色列表中的所有项
		this->SetRoleLstData(roleMap); // 设置角色列表中的内容
		this->OnSelchangeUserLst();
	}
}

BOOL CRoleManageDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)  
		return TRUE;
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)   
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

// OnClickTree1() : 处理在权限树上单击鼠标事件
void CRoleManageDlg::OnClickTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	DWORD dwpos = GetMessagePos(); // 获取鼠标单击事件
	TVHITTESTINFO ht = { 0 }; // 创建一个权限树项信息的对象

	// 设置信息对象的位置
	ht.pt.x = GET_X_LPARAM(dwpos);
	ht.pt.y = GET_Y_LPARAM(dwpos);

	::MapWindowPoints(HWND_DESKTOP, pNMHDR->hwndFrom, &ht.pt, 1);

	TreeView_HitTest(pNMHDR->hwndFrom, &ht); // 根据对象的位置信息获取项结点

	if (TVHT_ONITEMSTATEICON & ht.flags) { // 如果单击位置是在该项的checkbox中
		m_perThree.SelectItem(ht.hItem); // 设置该项为当前选中的项
		CheckChange(ht.hItem); // 调用处理函数
	}

	*pResult = 0;
}

void CRoleManageDlg::OnKeydownTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if(pTVKeyDown   &&   pTVKeyDown-> wVKey   ==   VK_SPACE) 
	{ 
		HTREEITEM selItem = m_perThree.GetSelectedItem();

		CheckChange(selItem);
	}

	*pResult = 0;
}

// CheckChange() : 当某一项checkbox的选中状态发生改变时的处理函数
// selItem : 所选择的项
void CRoleManageDlg::CheckChange(HTREEITEM &selItem)
{
	BOOL check = m_perThree.GetCheck(selItem); // 获取项的选中状态
	if (!check) { // 如果想要选中该项
		HTREEITEM parent = m_perThree.GetParentItem(selItem);
		if (parent) {
			check = m_perThree.GetCheck(parent); // 获取父结点的选中状态
			if (!check) {
				// 如果父结点没有选中，则该项也不能被选中
				// 这里第二个参数使用TRUE，因为树形结点被选中时状态变化的原因
				m_perThree.SetCheck(selItem, TRUE);
			}
		}
	} else { // 如果想要取消选中该项
		RecursionTree(selItem); // 递归结点，将该结点的所有子结点都取消选中
		// 取消选中该结点，这里第二个参数使用TRUE的原因同上
		m_perThree.SetCheck(selItem, TRUE);
	}
}
void CRoleManageDlg::OnDelUser() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_userLst.GetCurSel();
	int uId;

	if (nIndex == -1) {
		MessageBox("没有选择要删除的用户", "提示信息", MB_ICONWARNING);
		return;
	}

	uId = m_userLst.GetItemData(nIndex);
	if (ro.DelUser(uId)) {
		userMap.clear();
		ro.QueryUserByName(userMap);

		m_userLst.ResetContent();
		this->SetUserLstData(userMap);
		this->SetRoleLstNoCheck();
	}
}

void CRoleManageDlg::OnNewUser() 
{
	// TODO: Add your control notification handler code here
	CNewUserDlg newUser;
	
	newUser.DoModal();
}

void CRoleManageDlg::OnModiInfo() 
{
	// TODO: Add your control notification handler code here
	CNewUserDlg modiInfo;

	modiInfo.DoModal();
}

HBRUSH CRoleManageDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(nCtlColor==   CTLCOLOR_LISTBOX) {
		HBRUSH temp;

		pDC-> SetBkMode(TRANSPARENT); 
		pDC-> SetBkColor(RGB(223, 232, 238));       //设置背景 
		temp   =   CreateSolidBrush(RGB(223, 232, 238));

		return   temp; 
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}
