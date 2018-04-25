// CarDataDlg.cpp : implementation file
//
#include "stdafx.h"
#include "CarManage.h"
#include "CarDataDlg.h"
#include "EditCarDataDlg.h"
#include "ManageCarDlg.h"
#include "TMDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCarDataDlg dialog

CCarDataDlg::CCarDataDlg(CString &purviewCoding, CWnd *pParent /*=NULL*/)
	: CSizingDialog(CCarDataDlg::IDD, pParent), purviewCoding(purviewCoding)
{
	//{{AFX_DATA_INIT(CCarDataDlg)
	//}}AFX_DATA_INIT
}

void CCarDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CSizingDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCarDataDlg)
	DDX_Control(pDX, IDC_TAB1, m_manageTab);
	DDX_Control(pDX, IDC_EDIT2, m_searchText);
	DDX_Control(pDX, IDC_LIST1, m_carLst);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCarDataDlg, CSizingDialog)
	//{{AFX_MSG_MAP(CCarDataDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnSearch)
	ON_BN_CLICKED(IDC_BUTTON5, OnUpdate)
	ON_BN_CLICKED(IDC_BUTTON2, OnAddNew)
	ON_BN_CLICKED(IDC_BUTTON3, OnModify)
	ON_BN_CLICKED(IDC_BUTTON4, OnDel)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkCarLst)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCarDataDlg message handlers

BOOL CCarDataDlg::OnInitDialog()
{
	CSizingDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->SetWindowText(_T("�������Ϲ���"));

	m_carLst.SetExtendedStyle(LVS_EX_FULLROWSELECT); // �����б�ѡ��һ����
	m_carLst.SetBkColor(RGB(223, 232, 238));

	// ���ÿؼ���ʾ��TOOLTIP
	EnableToolTips(TRUE);
	m_toolTip.Create(this);
	m_toolTip.Activate(TRUE);
	m_toolTip.AddTool(GetDlgItem(IDC_EDIT2),
		"�����ؼ���:\'���ƺ�\',\'Ʒ���ͺ�\',\'��������\',\'��������\',\'��ʻԱ\',\'�ն˺�\'");

	// ����Ȩ�����ù����Ƿ����
	ModuleSetup(purviewCoding);

	AddCtrl();

	// �����б���ͷ
	m_carLst.InsertColumn(0, "����", LVCFMT_LEFT, 80);
	m_carLst.InsertColumn(1, "����", LVCFMT_LEFT, 100);
	m_carLst.InsertColumn(2, "Ʒ���ͺ�", LVCFMT_LEFT, 80);
	m_carLst.InsertColumn(3, "��������", LVCFMT_LEFT, 80);
	m_carLst.InsertColumn(4, "��������", LVCFMT_LEFT, 100);
	m_carLst.InsertColumn(5, "����״̬", LVCFMT_LEFT, 100);
	m_carLst.InsertColumn(6, "��ʻԱ", LVCFMT_LEFT, 100);
	m_carLst.InsertColumn(7, "��������", LVCFMT_LEFT, 100);
	m_carLst.InsertColumn(8, "�Ǽ�����", LVCFMT_LEFT, 100);
	m_carLst.InsertColumn(9, "�ն˺���", LVCFMT_LEFT, 100);

	// ����б������
	_RecordsetPtr res;
	carOpt.QueryAllCarInfo(res);
	FillCarLst(res);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// ModuleSetup() : ����Ȩ������ģ��Ĺ����Ƿ����
void CCarDataDlg::ModuleSetup(CString &purviewCoding)
{
	// ������볤��Ϊ�㣬ֱ�ӷ��أ����ٽ��й���ģ������
	// ������һ�����⣬��������Ϊ�յ�ʱ�����еĶ�����Ĭ��Ϊ���õ�
	CWnd* pWnd = this->GetWindow(GW_CHILD);   
    char cClassName[255] = { 0 }; 

	// ��ʼ�����ڿؼ���Ȩ��
    while(pWnd)
    {
		if (purviewCoding.GetLength() < 1) {
			return;
		}
		//�õ��ؼ�����������Ҫ��Edit��Button��Static�ȵ�
        GetClassName(pWnd->GetSafeHwnd(), cClassName, 255);
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

	// ��ʼ�� TAB �ؼ���Ȩ��
	int		codingLen = (purviewCoding.GetLength() > theApp.unit) ?
						theApp.unit : purviewCoding.GetLength();
	CString subPurviewCoding = purviewCoding.Left(codingLen);
	purviewCoding.Delete(0, codingLen);

	CDialog *dlg;
	for (int i = 0; i < codingLen; ++i) {
		if (subPurviewCoding.GetAt(i) == '1') {
			switch (i) {
			case 0:		// ���й������ɳ���Ȩ��
				dlg = new CManageCarDlg(purviewCoding);
				dlg->Create(IDD_MANAGECAR, &m_manageTab);
				m_manageTab.InsertLabel("������/�ɳ�", dlg);
				break;

			case 1:
				dlg = new CTMDlg(purviewCoding);
				dlg->Create(IDD_TMDLG, &m_manageTab);
				m_manageTab.InsertLabel("�ն˹���", dlg);
				break;

			default:
				break;
			}
		}
	}
}

// AddCtrl() : ���ô��ڵ����пؼ�ȫ����ӵ�����Ӧ���ڴ�С�Ŀؼ���
void CCarDataDlg::AddCtrl()
{
	int woc;
	HWND hwndChild = ::GetWindow(m_hWnd, GW_CHILD);

	while (hwndChild) {
		woc = ::GetDlgCtrlID(hwndChild);
		this->AddControl(woc, _T("RXY"));

		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
}

// FillCarLst() : ��䳵���б�����
// pRst : ���潫Ҫ���õ��б�����ݣ�Ϊһ���ݿ��¼��
void CCarDataDlg::FillCarLst(_RecordsetPtr &pRst)
{
	CString tmpStr;
	UINT row, col;

	m_carLst.DeleteAllItems();
	row = 0;
	while (!pRst->adoEOF) {
		col = 0;

		if (VT_NULL != pRst->GetCollect("coding").vt) { // �ж��ֶ�ֵ�Ƿ�Ϊ��
			tmpStr = pRst->GetCollect("coding").bstrVal;
		}
		m_carLst.InsertItem(row, tmpStr);
		tmpStr = "";
		m_carLst.SetItemData(row * 6, pRst->GetCollect("id").lVal);
		++col;

		if (VT_NULL != pRst->GetCollect("number").vt) {
			tmpStr = pRst->GetCollect("number").bstrVal;
		}
		m_carLst.SetItemText(row, col, tmpStr);
		tmpStr = "";
		++col;

		if (VT_NULL != pRst->GetCollect("carbrand").vt) {
			tmpStr = pRst->GetCollect("carbrand").bstrVal;
		}
		m_carLst.SetItemText(row, col, tmpStr);
		tmpStr = "";
		++col;

		if (VT_NULL != pRst->GetCollect("cartype").vt) {
			tmpStr = pRst->GetCollect("cartype").bstrVal;
		}
		m_carLst.SetItemText(row, col, tmpStr);
		tmpStr = "";
		++col;

		if (VT_NULL != pRst->GetCollect("belongdept").vt) {
			tmpStr = pRst->GetCollect("belongdept").bstrVal;
		}
		m_carLst.SetItemText(row, col, tmpStr);
		tmpStr = "";
		++col;

		if (VT_NULL != pRst->GetCollect("status").vt) {
			tmpStr = pRst->GetCollect("status").bstrVal;
		}
		m_carLst.SetItemText(row, col, tmpStr);
		tmpStr = "";
		++col;

		if (VT_NULL != pRst->GetCollect("driver").vt) {
			tmpStr = pRst->GetCollect("driver").bstrVal;
		}
		m_carLst.SetItemText(row, col, tmpStr);
		tmpStr = "";
		++col;

		if (VT_NULL != pRst->GetCollect("buytime").vt) {
			_variant_t date = pRst->GetCollect("buytime");
			tmpStr = COleDateTime(date).Format("%Y-%m-%d");
		}
		m_carLst.SetItemText(row, col, tmpStr);
		tmpStr = "";
		++col;

		if (VT_NULL != pRst->GetCollect("entertime").vt) {
			_variant_t date = pRst->GetCollect("entertime");
			tmpStr = COleDateTime(date).Format("%Y-%m-%d");	
		}
		m_carLst.SetItemText(row, col, tmpStr);
		tmpStr = "";
		++col;

		if (VT_NULL != pRst->GetCollect("terminalnum").vt) {
			tmpStr = pRst->GetCollect("terminalnum").bstrVal;
		}
		m_carLst.SetItemText(row, col, tmpStr);
		tmpStr = "";

		pRst->MoveNext();
		++row;
	}

	m_carLst.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED); // Ĭ��ѡ�е�һ��
}

void CCarDataDlg::OnSearch() 
{
	// TODO: Add your control notification handler code here
	CString searchText;
	_RecordsetPtr res;

	m_searchText.GetWindowText(searchText);

	carOpt.SearchByText(searchText, res);

	FillCarLst(res);
}


void CCarDataDlg::OnUpdate() 
{
	// TODO: Add your control notification handler code here
	_RecordsetPtr res;
	carOpt.QueryAllCarInfo(res);
	FillCarLst(res);
}

BOOL CCarDataDlg::PreTranslateMessage(MSG* pMsg) 
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

void CCarDataDlg::OnAddNew() 
{
	// TODO: Add your control notification handler code here
	CEditCarDataDlg ecd(TRUE); // ��������
	CStringArray res;

	if (ecd.DoModal() == IDOK) {
		ecd.GetContent(res);
		res.GetAt(0);
		m_carLst.AddToLstTail(res);
	}
}

void CCarDataDlg::OnModify() 
{
	// TODO: Add your control notification handler code here
	CStringArray content;
	UINT nIndex = m_carLst.GetNextItem(-1, LVNI_SELECTED);

	m_carLst.GetCurSelContent(content);
	CEditCarDataDlg ecd(FALSE, content, TRUE); // �޸���Ϣ����

	if (ecd.DoModal() == IDOK) {
		ecd.GetContent(content);
		UpdateRowContent(nIndex, content);
	}
}

void CCarDataDlg::OnDel() 
{
	// TODO: Add your control notification handler code here
	UINT nIndex = m_carLst.GetNextItem(-1, LVNI_SELECTED);

	if (nIndex == -1) {
		MessageBox("��û��ѡ���κ���", "��ʾ��Ϣ", MB_ICONWARNING);
		return;
	}

	CString number = m_carLst.GetItemText(nIndex, 1);

	if (AfxMessageBox("ȷ��Ҫɾ�����ƺ�Ϊ " + number + " �ĳ������� ?",
		MB_OK | MB_OKCANCEL) == IDOK) {
		if (carOpt.DelCarData(number)) {
			m_carLst.DeleteItem(nIndex);
		}
	}
}

// UpdateRowContent() : ����ĳһ�е�����
// index : ��Ҫ���µ�������
// content : ��������
//			��Ϊ���벻����£�����������ֱ�Ӵ� 1 ��ʼ
void CCarDataDlg::UpdateRowContent(const int &index,
								   const CStringArray &content)
{
	UINT size = content.GetSize();

	for (UINT col = 1; col < size; ++col) {
		m_carLst.SetItemText(index, col, content.GetAt(col));
	}
}

void CCarDataDlg::OnDblclkCarLst(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CStringArray content;

	m_carLst.GetCurSelContent(content);
	CEditCarDataDlg ecd(FALSE, content, FALSE); // ˫��������ʾ��Ϣ����
	
	ecd.DoModal();

	*pResult = 0;
}

void CCarDataDlg::OnSize(UINT nType, int cx, int cy) 
{
	CSizingDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (m_manageTab) {
		this->m_manageTab.OnSize(0, 0, 0);
	}
}