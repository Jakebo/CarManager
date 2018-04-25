// EditCarDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CarManage.h"
#include "EditCarDataDlg.h"
#include "CarOpt.h"
#include "DriverOpt.h"
#include "DeptOpt.h"
#include "TargetOpt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditCarDataDlg dialog


CEditCarDataDlg::CEditCarDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditCarDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditCarDataDlg)
	//}}AFX_DATA_INIT
}

CEditCarDataDlg::CEditCarDataDlg(BOOL addNew, CWnd *pParent /*=NULL*/)
	: CDialog(CEditCarDataDlg::IDD, pParent), addNew(addNew)
{
}

CEditCarDataDlg::CEditCarDataDlg(BOOL addNew,
								 const CStringArray &content,
								 BOOL editAble,
								 CWnd *pParent /*=NULL*/)
	: CDialog(CEditCarDataDlg::IDD, pParent), addNew(addNew), editAble(editAble)
{
	if (!addNew) { // ������޸Ļ�鿴���ϣ���Ҫ�޸Ļ�鿴�����ݱ��浽 content ��
		this->content.Copy(content);
	}
}

void CEditCarDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditCarDataDlg)
	DDX_Control(pDX, IDC_COMBO5, m_targetbox);
	DDX_Control(pDX, IDC_CARTYPE, m_cartypecombox);
	DDX_Control(pDX, IDC_CARBRAND, m_carbrandcombox);
	DDX_Control(pDX, IDC_BELONGDEPT, m_belongDept);
	DDX_Control(pDX, IDC_CARSTATUS, m_carstatus);
	DDX_Control(pDX, IDC_COMBO4, m_driverbox);
	//}}AFX_DATA_MAP
}

CEditCarDataDlg::~CEditCarDataDlg()
{
}

BEGIN_MESSAGE_MAP(CEditCarDataDlg, CDialog)
	//{{AFX_MSG_MAP(CEditCarDataDlg)
	ON_BN_CLICKED(IDOK, OnSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditCarDataDlg message handlers

BOOL CEditCarDataDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	carOpt = new CCarOpt();
	CWnd *pWnd = this->GetWindow(GW_CHILD);
	InitInterface();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// InitInterface() : ������������¼�����޸ļ�¼����ʼ�û�����
void CEditCarDataDlg::InitInterface()
{	
	if (addNew) { // �����������¼
		this->SetWindowText("������������"); // ���öԻ������
		// ���ݵ�ǰ���ں͵�ǰ������¼��ļ�¼�������ɱ��룬��֤�����Ψһ��
		COleDateTime date = COleDateTime::GetCurrentTime(); // ��ȡ��ǰ����
		int	count = carOpt->GetCountByDate(date); // ��ȡ��ǰ������¼�������
		CString countStr;

		countStr.Format("%04d", count);
		// ����"����"������
		GetDlgItem(IDC_CODING)->SetWindowText
			(date.Format("%Y%m%d") + countStr);
	} else { // ������޸Ļ�鿴����
		editAble ? SetWindowText("�޸ĳ�������") : SetWindowText("�鿴��������");
		SetContent(content);
	}

	GetDlgItem(IDC_CODING)->EnableWindow(FALSE); // ����"����"�򲻿ɱ༭
	InitCombox();
}

// InitCombox() : ��ʼ�� COMBOX �ؼ�
void CEditCarDataDlg::InitCombox()
{
	std::map<int, CString> list;
	std::map<int, CString>::iterator it, end;
	UINT nIndex;

	// ��ʼ����״̬�� COMBOBOX
	std::string status[6] = { "����", "����", "ά��", "ͣ��", "����", "����" };
	for (int i = 0; i < 6; ++i) {
		if ((nIndex = m_carstatus.AddString(status[i].data())) >= 0) {
			m_carstatus.SetItemData(i, i);
		}
	}

	// ��ʼ��ʻԱ�� COMBOBOX
	driverOpt = new CDriverOpt();
	driverOpt->QueryDriver(list);
	m_driverbox.SetContent(list);

	// ��ʼ���ŵ� COMBOBOX
	CDeptOpt deptOpt;
	list.clear();
	deptOpt.GetDept(list);
	m_belongDept.SetContent(list);

	// ��ʼ�������ͺͳ����̱�� COMBOBOX
	CStringArray strlist;
	carOpt->GetCarType(strlist);
	m_cartypecombox.SetContent(strlist);

	strlist.RemoveAll();
	carOpt->GetCarBrand(strlist);
	m_carbrandcombox.SetContent(strlist);

	// ��ʼ�ն˱���� COMBOBOX
	CTargetOpt targetOpt;
	strlist.RemoveAll();
	targetOpt.GetUnuseCoding(strlist);
	m_targetbox.SetContent(strlist);

	if (addNew) { // ����������������ϣ�����Ĭ�ϵ�ѡ��
		m_carstatus.SetCurSel(0);
		m_driverbox.SetCurSel(0);
	} else { // ����Ǳ༭�������ϣ����ó���״̬
		CString status;
		
		m_carstatus.GetWindowText(status);
		nIndex = m_carstatus.FindString(-1, status);

		m_carstatus.SetCurSel(nIndex);
	}
}

// GetContent() :  ��ȡ�û��������������
// res : ���淵�ؽ��
void CEditCarDataDlg::GetContent(CStringArray &content)
{
	content.Copy(this->content);
}

// SetContent() : ���ô������ݣ������޸Ļ�鿴����ʱ
// content : ��Ҫ���õ�����
void CEditCarDataDlg::SetContent(const CStringArray &content)
{
	CWnd *pWnd = this->GetWindow(GW_CHILD);
	UINT size = content.GetSize();
	char cClassName[255] = { 0 };

	// ����������ǰ size ���ؼ�����Ϊ content �洢������
	// ��һ��˳�����Ա����ؼ�ʱҲҪ����һ����˳��
	for (UINT i = 0; i < size; ++i) { 
		if (pWnd) {
			pWnd->EnableWindow(editAble); // ���ÿؼ��Ƿ�ɱ༭

			GetClassName(pWnd->GetSafeHwnd(), cClassName, 255);
			// �����ʱ��ؼ������ÿؼ���ʾ��ʱ��
			if (strcmp(cClassName, "SysDateTimePick32") == 0) {
				COleDateTime date;
				date.ParseDateTime(content.GetAt(i));
				((CDateTimeCtrl *)pWnd)->SetTime(date);

				pWnd = pWnd->GetWindow(GW_HWNDNEXT);
				continue;
			}

			pWnd->SetWindowText(content.GetAt(i)); // ֱ�����ÿؼ���ʾ����
			pWnd = pWnd->GetWindow(GW_HWNDNEXT); // ��һ�ؼ�
		}
	}

	GetDlgItem(IDOK)->EnableWindow(editAble);
}

// OnSave() : ����¼��ļ�¼
void CEditCarDataDlg::OnSave() 
{
	// TODO: Add your control notification handler code here
	CWnd *pWnd = this->GetWindow(GW_CHILD);
	char cClassName[255] = { 0 };
	std::string status[6] = { "����", "����", "ά��", "ͣ��", "����", "����" };
	COleDateTime date;
	CString text;
	
	// ���ؼ����Ƿ�Ϊ�գ����Ƿ������Ⱦ�����
	// ���ƺŲ���Ϊ��
	if (GetDlgItem(IDC_NUMBER)->GetWindowTextLength() == 0) {
		MessageBox("���ƺŲ���Ϊ��", "��ʾ��Ϣ", MB_ICONWARNING);
		return;
	}
	// ����״̬����Ϊ��
	if (GetDlgItem(IDC_CARSTATUS)->GetWindowTextLength() == 0) {
		MessageBox("����״̬����Ϊ��", "��ʾ��Ϣ", MB_ICONWARNING);
		return;
	}

	// ���������ն˺��룬����Ӧ�ն��Ƿ����
	// ��������

	// ��⳵�ƺ��Ƿ��ظ�
	GetDlgItem(IDC_NUMBER)->GetWindowText(text);
	if (addNew) {
		// ����������������ϣ�ֱ�Ӽ���Ƿ��ظ�
		if (carOpt->GetCountByNumber(text) > 0) {
			MessageBox("���ƺ�: " + text + "�Ѿ�����", "��ʾ��Ϣ", MB_ICONWARNING);
			return;
		}
	} else {
		// ������޸ĳ������ϣ��ȼ���Ƿ��޸��˳��ƺ�
		// ���û�޸�������⣬����޸����ټ���Ƿ��ظ�
		if (text.Compare(content.GetAt(1)) != 0) {
			if (carOpt->GetCountByNumber(text) > 0) {
				MessageBox("���ƺ�: " + text + "�Ѿ�����", "��ʾ��Ϣ", MB_ICONWARNING);
				return;
			}
		}

		content.RemoveAll(); // ��� CONTENT �����������
	}

	// �����Ⱦ���������б��浽 CONTENT ��
	while (pWnd) {
        GetClassName(pWnd->GetSafeHwnd(), cClassName, 255);
		if (strcmp(cClassName, "Button") != 0 &&
			strcmp(cClassName, "Static") != 0) {
			if (pWnd->GetDlgCtrlID() == IDC_CARSTATUS) {
				UINT nIndex = ((CComboBox*)pWnd)->GetCurSel();
				text.Format("%d", ((CComboBox*)pWnd)->GetItemData(nIndex));
			} else {
				pWnd->GetWindowText(text);
			}
			content.Add(text);
		}

		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}

	// ���ݱ��� addNew �������½����ϻ����޸�����
	if (addNew) {
		carOpt->NewCarData(content); // ���û�����������½������ݿ���
	} else {
		carOpt->ModiCarData(content); // ���޸ĺ�����ݸ��µ����ݿ�
	}

	int num = atoi((char*)LPCTSTR(content.GetAt(5)));
	content.SetAt(5, status[num].data());

	CDialog::OnOK();
}
