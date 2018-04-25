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
// FillLst() : ����б�����

BOOL CTMDlg::OnInitDialog() 
{
	CSizingDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_list.SetBkColor(RGB(223, 232, 238)); // ���ñ�����ɫ
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT); // �����б�ѡ��һ����
	
	ModuleSetup(m_purview);
	InitInterface();
	FillLst();
	AddCtrl();

	// ���ÿؼ���ʾ��TOOLTIP
	EnableToolTips(TRUE);
	m_toolTip.Create(this);
	m_toolTip.Activate(TRUE);
	m_toolTip.AddTool(GetDlgItem(IDC_EDIT1),
		"�����ؼ���:\'�ն���\',\'��ǰ��ַ\',\'��������\',\'�����û���\'");


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// ModuleSetup() : ����Ȩ������ģ��Ĺ����Ƿ����
void CTMDlg::ModuleSetup(CString &purviewCoding)
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

// AddCtrl() : ���ؼ���ӵ�����Ӧ��С��
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

// ��ʼ����
void CTMDlg::InitInterface()
{
	this->SetWindowText("�ն˹���");

	m_list.InsertColumn(0, _T("�ն˱���"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(1, _T("�ն���"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(2, _T("��ǰ��ַ"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(3, _T("֤��"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(4, _T("�ն˵绰"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(5, _T("Ȩ��"), LVCFMT_LEFT, 40);
	m_list.InsertColumn(6, _T("�Ƿ�����"), LVCFMT_LEFT, 60);
	m_list.InsertColumn(7, _T("��������"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(8, _T("�û���"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(9, _T("��ע"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(10, _T("�Ǽ�ʱ��"), LVCFMT_LEFT, 80);
}

// FillLst() : ����б�
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

// AddNewRow() : ���б������һ��
// content : ����һ�е�����
void CTMDlg::AddNewRow(const CStringArray &content)
{
	int row  = m_list.GetItemCount();

	m_list.InsertItem(row, ""); // ����һ��

	SetRowContent(row, content);
}

// GetRowContent() : ����һ��Ҫ���ȡѡ�е�ĳ�е�����
// content : ���沢���صõ�������
void CTMDlg::GetRowContent(CStringArray &content)
{
	// �Ȼ�ȡĳ�����е�����
	m_list.GetCurSelContent(content);

	// ��һ��Ҫ��Եõ������ݽ��д���
	content.RemoveAt(2);
	content.RemoveAt(5);
	content.RemoveAt(8);
}

// SetRowContent() : ����ĳ�е�����
// content : ��Ҫ���õ�����
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
