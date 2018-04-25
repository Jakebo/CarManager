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
	
	// ������δ���ɽ���޷����öԻ����ޱ߿򡢴�С���ɱ������
	DWORD dwStyle = GetStyle(); //��ȡ����ʽ  
    DWORD dwNewStyle = WS_OVERLAPPED | WS_VISIBLE | WS_SYSMENU | WS_CHILD | 
		WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;  
    dwNewStyle&=dwStyle; //��λ�뽫����ʽȥ��  
    SetWindowLong(this->m_hWnd, GWL_STYLE, dwNewStyle); //���ó��µ���ʽ  
    DWORD dwExStyle = GetExStyle(); //��ȡ����չ��ʽ  
    DWORD dwNewExStyle = WS_EX_LEFT |
		WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR;  
    dwNewExStyle&=dwExStyle; //��λ�뽫����չ��ʽȥ��  
    SetWindowLong(this->m_hWnd, GWL_EXSTYLE, dwNewExStyle); //�����µ���չ��ʽ  
	//����windows���ҵ���ʽ�ı��ˣ�����λ�úʹ�С����ԭ�����䣡
    SetWindowPos(NULL, 0, 0, 0, 0,
		SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);

	AddCtrl(); // �����ڵ����пؼ�����ӵ�����Ӧ���ڴ�С

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT); // �����б�ѡ��һ����
	m_list.SetBkColor(RGB(223, 232, 238));

	m_list.InsertColumn(0, "����", LVCFMT_LEFT, 80);
	m_list.InsertColumn(1, "����", LVCFMT_LEFT, 100);
	m_list.InsertColumn(2, "Ʒ���ͺ�", LVCFMT_LEFT, 80);
	m_list.InsertColumn(3, "��������", LVCFMT_LEFT, 80);
	m_list.InsertColumn(4, "��������", LVCFMT_LEFT, 100);
	m_list.InsertColumn(5, "����״̬", LVCFMT_LEFT, 100);
	m_list.InsertColumn(6, "��ʻԱ", LVCFMT_LEFT, 100);
	m_list.InsertColumn(7, "��������", LVCFMT_LEFT, 100);
	m_list.InsertColumn(8, "�Ǽ�����", LVCFMT_LEFT, 100);
	m_list.InsertColumn(9, "�ն˺���", LVCFMT_LEFT, 100);

	_RecordsetPtr pRst;
	CCarOpt carOpt;
	carOpt.QueryAllCarInfo(pRst);
	FillingInfo(pRst);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// SetTargetId() : �����б���еĵ�ǰѡ����Ϊָ������
// targetId : ��Ҫָ����target ID
void CCarInfo::SetListSel(const CString &coding)
{
	int nCount = m_list.GetItemCount(); // ��ȡ�б���е���Ŀ����
	CString text;
	
	if (nCount <= 0) {
		return;
	}

	// �����б�Ѱ�Ҹ�����һ�µ���
	for (int i = 0; i < nCount; ++i) {
		text = m_list.GetItemText(i, 9);  // ��ȡITEM TEXT
		if (coding == text) { // ƥ������
			// ���ö�Ӧ����Ϊѡ��״̬
			m_list.EnsureVisible(i, FALSE); // ȷ���б���ѡ�е��пɼ�
			m_list.SetFocus(); // �����б���ȡ����
			m_list.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED); // �����б����ѡ�е���
			break;
		}
	}
}

// FillingInfo() : ���ؼ�����Ϣ����
void CCarInfo::FillingInfo(_RecordsetPtr &pRst)
{
	CString tmpStr;
	UINT row, col;

	m_list.DeleteAllItems();
	row = 0;
	while (!pRst->adoEOF) {
		col = 0;

		if (VT_NULL != pRst->GetCollect("coding").vt) { // �ж��ֶ�ֵ�Ƿ�Ϊ��
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

	m_list.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED); // Ĭ��ѡ�е�һ��
}

// AddCtrl() : ���ô��ڵ����пؼ�ȫ����ӵ�����Ӧ���ڴ�С�Ŀؼ���
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
		// �رմ���
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
