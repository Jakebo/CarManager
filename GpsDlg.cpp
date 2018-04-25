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
	this->SetWindowText(_T("GPS����"));
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

// SetTreeData() : �������οؼ�������
// targetRecord : Ҫ�����οؼ����õ�����
void CGpsDlg::SetTreeData(std::vector<Target> &targetRecord)
{
	int nCount = targetRecord.size();
	HTREEITEM hTree; // ����
	HTREEITEM hSubTree; // �ӽ��
	CStringArray args; // ���� JS ����ʱ��Ҫ�Ĳ����б�
	CString targetIDStr; // ID �ַ���
	CString longitudeStr; // �����ַ���
	CString latitudeStr; // γ���ַ���

	Target tmp;
	// ��������
	for (int i = 0; i < nCount; ++i) { 
		tmp = targetRecord.at(i);
		hTree = m_tree.InsertItem(tmp.cargroupName, TVI_ROOT);

		int nums = tmp.targetLst.size();
		// ����������ĳ���
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

// CreateTraceWindow() : ���ݲ�ͬ�ı�־������ͬ�Ĵ���
// flag : ���ڱ�־��0 Ϊ���ٴ��ڣ�1 Ϊ�طŴ���
void CGpsDlg::CreateTraceWindow(char flag)
{
	HTREEITEM hItem = m_tree.GetSelectedItem(); // ��ȡ��ǰѡ����
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

// DeleteTraceDlg() : ɾ�����ٴ���,���ɸ��ٴ���
//				������Ϣ������
// iParam1 : ��һ����Ϣ����
// iParam2 : �ڶ�����Ϣ����
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

// DeletePBDlg() : ɾ���طŴ���,���ɻطŴ���
//				������Ϣ������
// iParam1 : ��һ����Ϣ����
// iParam2 : �ڶ�����Ϣ����
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

// DeleteCarInfoDlg() : ɾ��������Ϣ�Ĵ���,���ɳ�����Ϣ����
//					���ر�ʱ������Ϣ��������
// wParam : ��һ����Ϣ����
// lParam : �ڶ�����Ϣ���� 
LRESULT CGpsDlg::DeleteCarInfoDlg(WPARAM wParam, LPARAM lParam)
{
	if (carInfo != NULL) {
		delete carInfo;
		carInfo = NULL;
	}

	return 0;
}

// SetMarkerVisable() : ���õ�ͼ��ĳ����עΪ�ɼ�
LRESULT CGpsDlg::SetMarkerVisable(WPARAM wParam, LPARAM lParam)
{
	SetMarkerVisable(*((CString*)lParam));

	return 0;
}

// ���ø��ٴ��ڵ�λ��
void CGpsDlg::SetTraceWindowPos()
{
	int row = 0; // ���ٴ��ڵ�����
	int col = 0; // ���ٴ��ڵ�����
	float div = 2; // ���ٴ��ڴ�С��WEB���ڴ�С�ı�����ϵ
	CWebBrowser2 &web = ((CMapDlg*)this->GetParent()->GetParent())->GetWebBrowser();

	CRect rc;
	web.GetWindowRect(&rc); // ��ȡweb������

	std::map<int, CDialog* >::iterator traceDlgIt = m_traceDlg.begin();
	std::map<int, CDialog* >::iterator pBDlgIt = m_pBDlg.begin();

	while (traceDlgIt != m_traceDlg.end()) {
		CRect moveRc;
		if (traceDlgIt->second != NULL) {
			moveRc.left = rc.left + (5 + ((rc.Width() / div) * col));
			moveRc.right = rc.left + ((rc.Width() / div) * (col + 1));

			// ����Ѿ�����WEB���ڵ��ұ߽磬
			// ����+1������0�������¼������ұ߽��ֵ
			if (moveRc.right > rc.right) {
				++row;
				col = 0;

				moveRc.left = rc.left + (5 + ((rc.Width() / div) * col));
				moveRc.right = rc.left + ((rc.Width() / div) * (col + 1));
			}

			if (row == 0) { // ��������һ�У���bottom��20������
				moveRc.bottom = rc.bottom - (20 + ((rc.Height() / div) * row));
			} else { // ����Ϊ����µ�һ��5������
				moveRc.bottom = rc.bottom - (5 + ((rc.Height() / div) * row));
			}
			moveRc.top = rc.bottom - ((rc.Height() / div) * (row + 1));

			(traceDlgIt->second)->MoveWindow(moveRc); // �ƶ�����
			(traceDlgIt->second)->ShowWindow(SW_SHOW); // ��ʾ����

			++col;
			++traceDlgIt;
		}
	}

	while (pBDlgIt != m_pBDlg.end()) {
		CRect moveRc;
		if (pBDlgIt->second != NULL) {
			moveRc.left = rc.left + (5 + ((rc.Width() / div) * col));
			moveRc.right = rc.left + ((rc.Width() / div) * (col + 1));

			// ����Ѿ�����WEB���ڵ��ұ߽磬
			// ����+1������0�������¼������ұ߽��ֵ
			if (moveRc.right > rc.right) {
				++row;
				col = 0;

				moveRc.left = rc.left + (5 + ((rc.Width() / div) * col));
				moveRc.right = rc.left + ((rc.Width() / div) * (col + 1));
			}

			if (row == 0) { // ��������һ�У���bottom��20������
				moveRc.bottom = rc.bottom - (20 + ((rc.Height() / div) * row));
			} else { // ����Ϊ����µ�һ��5������
				moveRc.bottom = rc.bottom - (5 + ((rc.Height() / div) * row));
			}
			moveRc.top = rc.bottom - ((rc.Height() / div) * (row + 1));

			(pBDlgIt->second)->MoveWindow(moveRc); // �ƶ�����
			(pBDlgIt->second)->ShowWindow(SW_SHOW); // ��ʾ����

			++col;
			++pBDlgIt;
		}
	}
}

// SetCarInfoWndPos(): ���ó�����Ϣ���ڵ�λ��
void CGpsDlg::SetCarInfoWndPos()
{
	CRect rc;
	CWebBrowser2 &web = ((CMapDlg*)this->GetParent()->GetParent())->GetWebBrowser();

	web.GetWindowRect(&rc); // ��ȡweb������

	if (carInfo) {
		CRect tmp(rc.left + 5, (rc.bottom - rc.Width() / 6), 
							rc.right - 20, (rc.bottom - 20));
		carInfo->MoveWindow(&tmp);
		carInfo->ShowWindow(SW_SHOW);
	}
}

// CreateTraceMarker() : ������̬���ٻ�켣�طŵĳ�����־
// id : ����target��ID
void CGpsDlg::CreateTraceMarker(const int id)
{
	std::map<int, CDialog* >::iterator it = m_traceDlg.find(id);
	if (it != m_traceDlg.end()) {
		if (it->second) {
			((CTraceDlg*)it->second)->CreateTraceMarker();
		}
	}
}

// ModuleSetup() : ����Ȩ�ޱ������ö�Ӧ����ģ���Ƿ����
// purviewCoding : Ȩ�ޱ���
void CGpsDlg::ModuleSetup(CString &purviewCoding)
{
	// ������볤��Ϊ�㣬ֱ�ӷ��أ����ٽ��й���ģ������
	// ������һ�����⣬��������Ϊ�յ�ʱ�����еĶ�����Ĭ��Ϊ���õ�
	CWnd* pWnd = this->GetWindow(GW_CHILD);   
    char cClassName[255]={0};
    while(pWnd)
    {
		// ���Ȩ�ޱ����Ѿ����꣬�˳��������Ȩ��Ĭ��Ϊ����
		if (purviewCoding.GetLength() < 1) {
			break;
		}

		//�õ��ؼ�����������Ҫ��Edit��Button��Static�ȵ�
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

// ���οؼ��е�ѡ����ı�
void CGpsDlg::OnSelchangedCarTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (carInfo == NULL) {
		return;
	}

	HTREEITEM hItem = m_tree.GetSelectedItem();
	if (m_tree.ItemHasChildren(hItem)) { // ��������֤�����ǳ������
		return;
	}

	int targetId = m_tree.GetItemData(hItem);
	CString coding;
	targetOpt.QueryCodingById(targetId, coding);
	carInfo->SetListSel(coding);

	*pResult = 0;
}

// ˫�����οؼ��е�ITEM
void CGpsDlg::OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	HTREEITEM hItem = m_tree.GetSelectedItem(); // ��ȡ��ǰѡ����
	if (m_tree.ItemHasChildren(hItem)) {
		return;
	}

	int targetId = m_tree.GetItemData(hItem);
	SetMarkerVisable(targetId);

	*pResult = 0;
}

// OnCarInfo() : �鿴������Ϣ�¼�
void CGpsDlg::OnCarInfo() 
{
	// TODO: Add your control notification handler code here
	if (carInfo == NULL) { // �����û�д���������Ϣ���ڣ��򴴽�
		carInfo = new CCarInfo();
		carInfo->Create(IDD_CARINFO, GetDlgItem(IDD_DIALOG1));
		SetCarInfoWndPos();

		// ����ΪʲôҪ���´��ڣ���������£��½�������
		// ������Ϣ�Ի���Ϊʲô�п��ܲ�����ȷ��ʾ
		this->GetParent()->Invalidate();
	}

	HTREEITEM hItem = m_tree.GetSelectedItem(); // ��ȡ��ǰѡ����
	if (m_tree.ItemHasChildren(hItem)) {
		return;
	}

	int targetID = m_tree.GetItemData(hItem);
	carInfo->SetListSel(targetID); // ������Ϣ�����г����б��ѡ����
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
	// ���ݶ���TAB�����ö�̬���ٴ��ڵ���ʾ���
	std::map<int, CDialog* >::iterator begin = m_traceDlg.begin();
	std::map<int, CDialog* >::iterator end	 = m_traceDlg.end();
	UINT show = bShow ? SW_SHOW : SW_HIDE;

	while (begin != end) {
		begin->second->ShowWindow(show);
		++begin;
	}

	// ���ݶ���TAB�����ù켣�طŴ��ڵ���ʾ���
	begin = m_pBDlg.begin();
	end	  = m_pBDlg.end();
	
	while (begin != end) {
		begin->second->ShowWindow(show);
		++begin;
	}

	// ����Ѵ��˳�����Ϣ���ڣ�Ҳ������ʾ���
	if (carInfo) {
		carInfo->ShowWindow(show);
	}
}

// SetMarkerVisable() : �����ն� ID �����õ�ͼ�ϵ��ն˱�ע�Ƿ�ǰ�ɼ�
// targetId : ָ�����ն� ID
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

// SetMarkerVisable() : �����ն˱��������õ�ͼ�ϵ��ն˱�ע�Ƿ�ǰ�ɼ�
// targetCoding : ָ�����ն˱���
void CGpsDlg::SetMarkerVisable(const CString &targetCoding)
{
	int id = targetOpt.QueryIdByCoding(targetCoding);
	
	SetMarkerVisable(id);
}
