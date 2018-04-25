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

// ���ó�ԱҪ�ڳ�ʼ���б����ʼ��
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
	this->SetWindowText(_T("Ȩ�޹���"));

	// Ȩ��Ȩ�����ø�ģ�鹦���Ƿ����
	ModuleSetup(purviewCoding);

	// ���ý�ɫ�б�ķ��Ϊ��CHECKBOX
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

// SetRoleLstData() : ���ý�ɫ�б������
// roleMap : ��Ҫ���õĽ�ɫ�б��MAP����
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
	// ��ȡ������TAB�ؼ�
	CMyTabCtrl *pTab = (CMyTabCtrl *)this->GetParent();
	// ȡ�ö���TAB�ؼ���������д���
	std::map<int, CDialog *> dlgMap = pTab->GetDlgMap();
	std::map<int, CDialog *>::iterator it = dlgMap.begin();
	HTREEITEM hTree;

	while (it != dlgMap.end()) {
		// ��������ȡ����Ȩ���йصĿؼ�������Ȩ��������ʾ����
		GetWndChild(FALSE, hTree, it->second);

		++it;
	}
}

// GetWndChild() : �������ڵĿؼ���ͬʱ����Ӧ������ӵ�Ȩ������
// parentItem : �ݹ��е����ĸ����
// pWnd : ��Ҫ�����Ĵ���ָ��
void CRoleManageDlg::GetWndChild(BOOL haveParent,
								 HTREEITEM &parentItem,
								 CWnd *pWnd)
{
	if (!pWnd) {
		return;
	}

	CWnd *childWnd = pWnd->GetWindow(GW_CHILD); // ��ȡ���ڵ��ӿؼ�
	char cClassName[255] = { 0 };
	HTREEITEM hTree;
	HTREEITEM childItem;
	CString text;

	pWnd->GetWindowText(text);
	if (haveParent) { // �и����ڣ�Ϊ����
		hTree = m_perThree.InsertItem(text, parentItem);
	} else { // ����Ϊ��
		hTree = m_perThree.InsertItem(text, TVI_ROOT);
	}
	// ���������������
	m_perThree.SetItemData(hTree,
		theApp.topDlgLst[pWnd->GetDlgCtrlID()]);

	while (childWnd) {
		// ��ȡ�ؼ�������
		GetClassName(childWnd->GetSafeHwnd(), cClassName, 255);
		// �������ΪButton����ӵ����в�������Ӧ�������
		// �������Ϊsystabcontrol32�������������TAB�ؼ�������Ĵ���
		if (strcmp(cClassName, "Button") == 0) {
			// �ж��Ƿ��� PUSHBUTTON,��Ϊ RADIOBUTTON �� CHECKBUTTON
			// ������Ҳ�� CButton
			if (((CButton *)childWnd)->GetButtonStyle() == BS_PUSHBUTTON) {
				childWnd->GetWindowText(text); // ��ȡ��ť�ϵ��ı�

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
	m_perThree.Expand(hTree, TVE_EXPAND); // չ����
}

// SetRoleLstCheck() : �����б��е�ѡ�еĹ���Ա��
//			���Ľ�ɫ�����ý�ɫ�б����Check״̬
// userRoleMap : �б���ѡ�й���Ա�����Ľ�ɫ
// ʵ�ַ�����������������userRoleMap����������RoleLst�в��Ҷ�Ӧ��ƥ����
//		�������ƥ�����ѡ��ƥ������û�У������
void CRoleManageDlg::SetRoleLstCheck(std::map<int, CString> &userRoleMap)
{
	LVFINDINFO info;
	int nIndex;
	int len = userRoleMap.size();
	std::map<int, CString>::iterator it = userRoleMap.begin();

	info.flags = LVFI_STRING; // Fine entire match
	
	for (int i = 0; i < len; ++i) {
		info.psz = it->second;
		nIndex = m_roleLst.FindItem(&info); // ���Ҷ�Ӧ����
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

// SetPerTreeCheck() : ����Ȩ�ޱ���������Ȩ������ѡ��״̬
// hTree : ��Ҫ�����������
// purviewCoding : Ȩ�ޱ���
void CRoleManageDlg::SetPerTreeCheck(HTREEITEM &hTree,
									 CString &purviewCoding)
{
	HTREEITEM hasChildItem;
	int nCount = 0;

	// ����ṩ������㲻Ϊ�գ�������ý�㼰�����ֵܽ��
	while (hTree) {
		if (m_perThree.ItemHasChildren(hTree)) { // �������ĳ������ӽ��(��ʾ����TAB)
			if (nCount == 0) {
				hasChildItem = hTree; // �����һ�����������ӽ�����
			}
			++nCount;
		}

		// ����Ѿ�û�б��룬����
		if (purviewCoding.GetLength() < 1) {
			return;
		}
		// ����ý���Ӧ�ı���Ϊ1������Ϊѡ��״̬
		if (purviewCoding.GetAt(0) == '1') {
			m_perThree.SetCheck(hTree, TRUE);
		}

		purviewCoding.Delete(0); // ɾ��һ������
		hTree = m_perThree.GetNextSiblingItem(hTree); // ��ȡ��һ���ֵܽ��
	}

	// ����������ӽ������������д������ӽ�����
	if (nCount != 0) {
		// ���������TAB�����ڵĸ�������һ����Ԫ���򲹹�һ����Ԫ
		purviewCoding.Delete(0, theApp.unit - nCount);
		HTREEITEM childItem;
		do {
			childItem = m_perThree.GetChildItem(hasChildItem);
			// �����ӽ��
			SetPerTreeCheck(childItem, purviewCoding);
			// ��ȡ��һ�ֵܽ��
			hasChildItem = m_perThree.GetNextSiblingItem(hasChildItem); 
		} while (hasChildItem);
	}
}

// Set tree's item status no check
void CRoleManageDlg::SetPerTreeNoCheck()
{
	HTREEITEM root = m_perThree.GetRootItem(); // ��ȡȨ�����ĸ�

	while (root != NULL) {
		RecursionTree(root); // �ݹ���
		root = m_perThree.GetNextSiblingItem(root);
	} 
}

// GetPerTreeCheck() : ��ȡȨ������ѡ�е������Ȩ�ޱ���
void CRoleManageDlg::GetPerTreeCheck(CString &purviewCoding)
{
	purviewCoding = _T("");
	HTREEITEM root = m_perThree.GetRootItem();

	RecursionForGetCheck(root, purviewCoding);
}

// RecursionForGetCheck() : ��ȡȨ������ѡ��״̬������������Ȩ������
//					ѡ��״̬���
// hTree :  ��Ҫ������ȡ�������
// purviewCoding : �����ȡ���ı���
void CRoleManageDlg::RecursionForGetCheck(HTREEITEM &hTree,
										  CString &purviewCoding)
{
	if (!hTree) { // �������Ϊ�գ�����
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
		// ������Ϊѡ��״̬�����Ӧ����Ϊ1������Ϊ0
		if (m_perThree.GetCheck(hTree)) {
			purviewCoding += "1";
		} else {
			purviewCoding += "0";
		}
		hTree = m_perThree.GetNextSiblingItem(hTree);
	}

	if (nCount != 0) {
		// �������һ��TAB�����ڵĵ�Ԫ������һ����Ԫ
		for (int i = (theApp.unit - nCount); i > 0; --i) {
			purviewCoding += "0";
		}

		HTREEITEM childItem; // ��������
		do {
			childItem = m_perThree.GetChildItem(hasChildItem);
			RecursionForGetCheck(childItem, purviewCoding);
			hasChildItem = m_perThree.GetNextSiblingItem(hasChildItem);
		} while (hasChildItem);
	}
}

// ModuleSetup() : ����Ȩ������ģ��Ĺ����Ƿ����
void CRoleManageDlg::ModuleSetup(CString &purviewCoding)
{
	// ������볤��Ϊ�㣬ֱ�ӷ��أ����ٽ��й���ģ������
	// ������һ�����⣬��������Ϊ�յ�ʱ�����еĶ�����Ĭ��Ϊ���õ�
	CWnd* pWnd = this->GetWindow(GW_CHILD);
    char cClassName[255]={0}; 
    while(pWnd)
    {   
		if (purviewCoding.GetLength() < 1) {
			return;
		}

		//�õ��ؼ�����������Ҫ��Edit��Button��Static�ȵ�
        GetClassName(pWnd->GetSafeHwnd(),cClassName,255);
		
		if (strcmp(cClassName, "Button") == 0) {
			// ����ؼ���Ӧ�ı���Ϊ1��Ϊ���ã����򲻿���
			if (purviewCoding.GetAt(0) == '1') {
				pWnd->EnableWindow();
			} else {
				pWnd->EnableWindow(FALSE);
			}
			purviewCoding.Delete(0); // ɾ���������ı���
		}

        pWnd = pWnd->GetWindow(GW_HWNDNEXT); // ��һ�ؼ�
     }
}

// RecursionTree() : ʹ�õݹ��������������ṩ�ĸ����,�����ñ���
//					�������ѡ��״̬Ϊ��(��ѡ��)
// hRoot : ��Ҫ�����ĸ����
void CRoleManageDlg::RecursionTree(HTREEITEM &hRoot)
{
	m_perThree.SetCheck(hRoot, FALSE); // ���ý���ѡ��״̬Ϊ��

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
	POINT Newp; //��ȡ���ڶԻ���Ĵ�С
	CRect recta;

	GetClientRect(&recta);     //ȡ�ͻ�����С
	Newp.x=recta.right-recta.left;
	Newp.y=recta.bottom-recta.top;
	fsp[0]=(float)Newp.x/old.x;
	fsp[1]=(float)Newp.y/old.y;

	CRect Rect;
	int woc;
	CPoint OldTLPoint,TLPoint; //���Ͻ�
	CPoint OldBRPoint,BRPoint; //���½�

	HWND  hwndChild=::GetWindow(m_hWnd,GW_CHILD);  //�г����пؼ�
	while(hwndChild)  
	{  
		woc=::GetDlgCtrlID(hwndChild);//ȡ��ID
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
		int roleID = m_roleLst.GetItemData(nIndex); // ��ȡ��ɫID

		CString purviewCoding;
		ro.QueryPermissByRID(roleID, purviewCoding); // ��ѯ��Ӧ��ɫ��Ȩ�ޱ���

		HTREEITEM root = m_perThree.GetRootItem();
		
		SetPerTreeNoCheck();
		if (purviewCoding.GetLength() == 0) { // ���Ȩ�ޱ���Ϊ�գ�Ȩ����ȫ��ѡ
			return;
		}

		SetPerTreeCheck(root, purviewCoding); // ����Ȩ������ѡ��״̬
	}

	*pResult = 0;
}

// OnSavePermission() : ���"����Ȩ��"��ť�¼�
//				���û�������Ȩ���޸Ľ��б���
void CRoleManageDlg::OnSavePermission() 
{
	// TODO: Add your control notification handler code here
	CString purviewCoding;

	SaveMRRelation(); // �����û�-��ɫ�Ĺ�ϵ
	this->GetPerTreeCheck(purviewCoding); // ��ȡȨ�����е�ѡ��״̬
	SaveRolePurviewCoding(purviewCoding); // �����ɫ-Ȩ�ޱ����ϵ
}

// SaveMRrelation() : ����ı�֮��Ĺ���Ա-��ɫ��ϵ
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

// SaveRolePurviewCoding() : ����ı�֮��Ľ�ɫ��Ӧ��Ȩ�ޱ���
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


// GetDiffRoleAtRoleLst() : ����ͨ����ɫ�б���ѡ�������ж�ĳ
//		����Ա�޸ĺ�Ľ�ɫ��ԭ�еĽ�ɫ�Ĳ�ͬ���Ծ�����ɫ����ɾ
// roleLst : ��ɫ�б�
// addMR : ������ҪΪĳ������Ա������ɫ��ID
// delMR : ������ҪΪĳ������Աɾ����ɫ��ID
// URMap : ĳ����Աԭ�н�ɫ��ID
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

// OnNewRole() : ������ɫ
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

// OnDelRole() : ɾ���б���ѡ����ɫ
void CRoleManageDlg::OnDelRole() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_roleLst.GetNextItem(-1, LVNI_SELECTED);

	if (nIndex == -1) {
		MessageBox("��û��ѡ���κν�ɫ", "��ʾ��Ϣ", MB_ICONWARNING);
		return;
	}

	if (AfxMessageBox("�Ƿ�ɾ����ɫ " + m_roleLst.GetItemText(nIndex, 0) + " ?",
		MB_OK | MB_OKCANCEL) == IDCANCEL) {
		return;
	}

	int rID = m_roleLst.GetItemData(nIndex); // ��ȡѡ�����ID
	if (ro.DelRole(rID)) { // ɾ��ѡ�е���
		roleMap.clear(); // �����ɫ��ѯ��¼
		ro.QueryRole(roleMap); // ���²�ѯɾ����ɫ��ļ�¼

		m_roleLst.DeleteAllItems(); // ɾ����ɫ�б��е�������
		this->SetRoleLstData(roleMap); // ���ý�ɫ�б��е�����
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

// OnClickTree1() : ������Ȩ�����ϵ�������¼�
void CRoleManageDlg::OnClickTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	DWORD dwpos = GetMessagePos(); // ��ȡ��굥���¼�
	TVHITTESTINFO ht = { 0 }; // ����һ��Ȩ��������Ϣ�Ķ���

	// ������Ϣ�����λ��
	ht.pt.x = GET_X_LPARAM(dwpos);
	ht.pt.y = GET_Y_LPARAM(dwpos);

	::MapWindowPoints(HWND_DESKTOP, pNMHDR->hwndFrom, &ht.pt, 1);

	TreeView_HitTest(pNMHDR->hwndFrom, &ht); // ���ݶ����λ����Ϣ��ȡ����

	if (TVHT_ONITEMSTATEICON & ht.flags) { // �������λ�����ڸ����checkbox��
		m_perThree.SelectItem(ht.hItem); // ���ø���Ϊ��ǰѡ�е���
		CheckChange(ht.hItem); // ���ô�����
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

// CheckChange() : ��ĳһ��checkbox��ѡ��״̬�����ı�ʱ�Ĵ�����
// selItem : ��ѡ�����
void CRoleManageDlg::CheckChange(HTREEITEM &selItem)
{
	BOOL check = m_perThree.GetCheck(selItem); // ��ȡ���ѡ��״̬
	if (!check) { // �����Ҫѡ�и���
		HTREEITEM parent = m_perThree.GetParentItem(selItem);
		if (parent) {
			check = m_perThree.GetCheck(parent); // ��ȡ������ѡ��״̬
			if (!check) {
				// ��������û��ѡ�У������Ҳ���ܱ�ѡ��
				// ����ڶ�������ʹ��TRUE����Ϊ���ν�㱻ѡ��ʱ״̬�仯��ԭ��
				m_perThree.SetCheck(selItem, TRUE);
			}
		}
	} else { // �����Ҫȡ��ѡ�и���
		RecursionTree(selItem); // �ݹ��㣬���ý��������ӽ�㶼ȡ��ѡ��
		// ȡ��ѡ�иý�㣬����ڶ�������ʹ��TRUE��ԭ��ͬ��
		m_perThree.SetCheck(selItem, TRUE);
	}
}
void CRoleManageDlg::OnDelUser() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_userLst.GetCurSel();
	int uId;

	if (nIndex == -1) {
		MessageBox("û��ѡ��Ҫɾ�����û�", "��ʾ��Ϣ", MB_ICONWARNING);
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
		pDC-> SetBkColor(RGB(223, 232, 238));       //���ñ��� 
		temp   =   CreateSolidBrush(RGB(223, 232, 238));

		return   temp; 
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}
