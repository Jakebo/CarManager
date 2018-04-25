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
	// ������δ���ɽ���޷����öԻ����ޱ߿򡢴�С���ɱ������
	DWORD dwStyle = GetStyle(); //��ȡ����ʽ  
    DWORD dwNewStyle = WS_OVERLAPPED | WS_VISIBLE | WS_SYSMENU | WS_CHILD | 
		WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;  
    dwNewStyle&=dwStyle; //��λ�뽫����ʽȥ��  
    SetWindowLong(this->m_hWnd, GWL_STYLE, dwNewStyle); //���ó��µ���ʽ 1073741892 
    DWORD dwExStyle = GetExStyle(); //��ȡ����չ��ʽ  
    DWORD dwNewExStyle = WS_EX_LEFT |
		WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR;
    dwNewExStyle&=dwExStyle; //��λ�뽫����չ��ʽȥ��
    SetWindowLong(this->m_hWnd, GWL_EXSTYLE, dwNewExStyle); //�����µ���չ��ʽ  
	//����windows���ҵ���ʽ�ı��ˣ�����λ�úʹ�С����ԭ�����䣡
    SetWindowPos(NULL, 0, 0, 0, 0,
		SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);

	this->SetWindowText("������/�ɳ�");
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT); // �����б�ѡ��һ����
	m_list.SetBkColor(RGB(223, 232, 238)); // ���ñ�����ɫ

	AddCtrl(); // ����Զ���Ӧ�ؼ�
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);

	m_ioOpt = new CInOutOpt();

	// ��������ʱ��ؼ��ĳ�ʼʱ�䣬����� DEBUG �汾
	// Ĭ�ϳ�ʼʱ��Ϊ��ǰʱ�䣬������� RELEASE �汾
	// Ĭ�ϵĳ�ʼʱ��ȴ���ǵ�ǰʱ�䣬����Ϊʲô��
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

// ModuleSetup() : ����Ȩ������ģ��Ĺ����Ƿ����
void CManageCarDlg::ModuleSetup(CString &purviewCoding)
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
			if (((CButton *)pWnd)->GetButtonStyle() == 
				BS_PUSHBUTTON) {
				// ����ؼ���Ӧ�ı���Ϊ1��Ϊ���ã����򲻿���
				if (purviewCoding.GetAt(0) == '1') {
					pWnd->EnableWindow();
				} else {
					pWnd->EnableWindow(FALSE);
				}
				purviewCoding.Delete(0); // ɾ���������ı���
			}
		}

        pWnd = pWnd->GetWindow(GW_HWNDNEXT); // ��һ�ؼ�
     }
}

// InitList() : ��ʼ���б�
void CManageCarDlg::InitList()
{
	m_list.InsertColumn(0, _T("����"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(1, _T("���ƺ�"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(2, _T("��ʻԱ"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(3, _T("��ʻԱ�绰"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(4, _T("�ó���"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(5, _T("�ó��˵绰"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(6, _T("�ó�����"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(7, _T("�ó�ʱ��"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(8, _T("���Ʒ���ʱ��"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(9, _T("�����ص�"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(10, _T(";���ص�"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(11, _T("Ŀ�ĵص�"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(12, _T("�ó�ԭ��"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(13, _T("�ɳ���"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(14, _T("������"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(15, _T("�Ǽ�����"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(16, _T("�Ƿ����ճ�"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(17, _T("�ճ���"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(18, _T("��ע"), LVCFMT_LEFT, 100);

	_RecordsetPtr res;
	m_ioOpt->QueryRecord(res);

	FillList(res);
}

// FillList() : ���б����������
// res : ��ѯ���ݿ�õ��ļ�¼��
void CManageCarDlg::FillList(_RecordsetPtr &res)
{
	UINT row;

	m_list.DeleteAllItems();

	row = 0;
	while (!res->adoEOF)
	{
		// ����ǰ 7 ���е�����
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

		// ���� 7��8 �е�ʱ������
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

		// ���ú����е�����
		for (col = 9; col < 19; ++col) {
			if (col != 15) {
				if (VT_NULL != res->GetCollect(long(col)).vt) {
					m_list.SetItemText(row, col,
						(CString)res->GetCollect(long(col)).bstrVal);
				} else {
					m_list.SetItemText(row, col, "");
				}
			} else { // ����ǵǼ�ʱ���У����ȡʱ��
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

// AddToLstTail() : ��һ����¼��ӵ��б�β��
// content : ��¼����
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

// UpdateLstRow() : ����ĳ�м�¼������
// nIndex : ָ����ĳ��
// content : ��Ҫ���µ�����
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

// GetIndexContent() : ��ȡĳ�е�����
// nIndex : ָ����һ�� 
// content : ����ĳ�е�����
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

	// ��ȡ�ؼ�����
	time = m_starttime.Format("%Y-%m-%d"); // ��ʼʱ��
	if (!time.IsEmpty()) {
		searchText += " usetime>='" + time + "' and";
		++count;
	}
	time = m_endtime.Format("%Y-%m-%d"); // ����ʱ��
	if (!time.IsEmpty()) {
		searchText += " usetime<='" + time + "' and";
		++count;
	}

	if (!m_dest.IsEmpty()) { // Ŀ�ĵ�
		searchText += " destination like '%" + m_dest + "%' and";
		++count;
	}
	if (!m_user.IsEmpty()) { // �ó���
		searchText += " [user] like '%" + m_user + "%' and";
		++count;
	}
	if (!m_dept.IsEmpty()) { // ����
		searchText += " dept in (select id from depts where \
						name like '%" + m_dept + "%') and";
		++count;
	}

	if (count > 0) { // ����û��Ѿ������ѯ��������ѯ
		searchText.Delete(searchText.GetLength() - 4, 4);
		m_ioOpt->QueryRecord(res, (LPCTSTR)searchText, m_radSelect); // ��ѯ
	} else { // ����û�û�������κβ�ѯ��������ˢ���б�
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
	if (m_radSelect == 0) { // ������ı�ѡ��ֱ�ӷ��ز�������
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
		MessageBox("û��ѡ���κ���", "��ʾ��Ϣ", MB_ICONWARNING);
		return;
	}

	// �ȼ��ó����Ƿ��Ѿ��ճ�
	CString backstatus = m_list.GetItemText(nIndex, 16);
	if (backstatus.Compare("����") == 0) {
		MessageBox("������Ϊ: " + m_list.GetItemText(nIndex, 1) +
					" �ĳ������ճ�", "��ʾ��Ϣ", MB_OK);
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
