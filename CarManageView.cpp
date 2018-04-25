// CarManageView.cpp : implementation of the CCarManageView class
//

#include "stdafx.h"
#include "CarManage.h"

#include "CarManageDoc.h"
#include "CarManageView.h"

#include "MapDlg.h"
#include "RoleManageDlg.h"
#include "CarDataDlg.h"
#include "LogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCarManageView

CMapDlg *mapDlg;

IMPLEMENT_DYNCREATE(CCarManageView, CFormView)

BEGIN_MESSAGE_MAP(CCarManageView, CFormView)
	//{{AFX_MSG_MAP(CCarManageView)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_MODIPW, OnModipw)
	ON_BN_CLICKED(IDC_RELOGIN, OnRelogin)
	ON_BN_CLICKED(IDC_EXIT, OnExit)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCarManageView construction/destruction

CCarManageView::CCarManageView()
	: CFormView(CCarManageView::IDD)
{
	//{{AFX_DATA_INIT(CCarManageView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here
	m_curSelIndex = -1;
}

CCarManageView::~CCarManageView()
{
	std::map<DialogType, CDialog*>::iterator it = m_dlgMap.begin();

	while (it != m_dlgMap.end()) {
		delete it->second;
		++it;
	}
}

void CCarManageView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCarManageView)
	DDX_Control(pDX, IDC_TAB1, m_tab);
	//}}AFX_DATA_MAP
}

BOOL CCarManageView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CCarManageView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	// ��ѯ��¼�û���Ȩ�ޱ���
	CString purviewCoding;
	ro.QueryUserPurviewByName(CLogDlg::GetID(), purviewCoding);
	// ����Ȩ�ޱ���������ϵͳ�ĸ�������ģ���Ƿ����
	ModuleSetup(purviewCoding);

	// ���õ�¼��Ϣ
	(GetDlgItem(IDC_WELCOMELAB))->SetWindowText(CLogDlg::GetName() + "  ��ӭ��");
}

/////////////////////////////////////////////////////////////////////////////
// CCarManageView printing

BOOL CCarManageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCarManageView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCarManageView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CCarManageView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CCarManageView diagnostics

#ifdef _DEBUG
void CCarManageView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCarManageView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CCarManageDoc* CCarManageView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCarManageDoc)));
	return (CCarManageDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCarManageView message handlers

void CCarManageView::ModuleSetup(CString &purviewCoding)
{
	CString subPurview = purviewCoding.Left(theApp.unit);
	purviewCoding.Delete(0, theApp.unit);
	
	// ����TAB   ROLEDLG MAPDLG T  GPS   
	for (int i = 0; i < theApp.unit; ++i) {
		if (subPurview.GetLength() < 1) {
			break;
		}

		if (subPurview.GetAt(i) == '1') {
			switch (i) {
			case 0:
				if (m_dlgMap[CARDATADLG] == NULL) {
					m_dlgMap[CARDATADLG] = new CCarDataDlg(purviewCoding);
					m_dlgMap[CARDATADLG]->Create(IDD_CARDATADLG, &m_tab);

					m_tab.InsertLabel("�������Ϲ���", m_dlgMap[CARDATADLG]);
				}
				break;
			case 1:
				if (m_dlgMap[ROLEMANAGEDLG] == NULL) {
					m_dlgMap[ROLEMANAGEDLG] = new CRoleManageDlg(purviewCoding);
					m_dlgMap[ROLEMANAGEDLG]->Create(IDD_ROLEDLG, &m_tab);
					
					m_tab.InsertLabel("Ȩ�޹���", m_dlgMap[ROLEMANAGEDLG]);	
				}
				break;
			case 2:
				if (m_dlgMap[MAPDLG] == NULL) {
					mapDlg = new CMapDlg(purviewCoding);
					m_dlgMap[MAPDLG] = mapDlg;
					m_dlgMap[MAPDLG]->Create(IDD_DIALOG1, &m_tab);

					m_tab.InsertLabel("��ͼ", m_dlgMap[MAPDLG]);
				}
				break;
			default:
				break;
			}
		}
	}

	// ����õ�¼���û�ӵ��Ȩ�޹����Ȩ�ޣ�����ʾȨ�����е�����
	// ������δ��벻Ӧ�������Ӧ��������Ĵ����У�����ʱ��������
	if (m_dlgMap[ROLEMANAGEDLG]) {
		((CRoleManageDlg *)m_dlgMap[ROLEMANAGEDLG])->SetPerTreeData();
	}
}

void CCarManageView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	// Set the tab & all dialog
	CRect rc;
	GetClientRect(rc);
	
	if (m_tab) {
		m_tab.MoveWindow(&rc);
		m_tab.OnSize(0, 0, 0);
	}

	// ���� Static �ؼ��Ե������ǵ�λ��
	if (GetDlgItem(IDC_EXIT)) {
		CRect	labRc;
		int		moveDec;

		// ����"�˳�"λ��
		GetDlgItem(IDC_EXIT)->GetClientRect(&labRc); // ��ȡ�ؼ��Ŀͻ�����

		labRc.left = rc.right - labRc.Width(); // ������������λ��
		labRc.right = rc.right;

		GetDlgItem(IDC_EXIT)->MoveWindow(labRc); // ���ÿؼ�λ��
		moveDec = rc.right - labRc.Width() - 5;

		// �����������ָ��ߵ�λ��
		GetDlgItem(IDC_THREE)->GetClientRect(&labRc);

		labRc.left = moveDec - labRc.Width();
		labRc.right = moveDec;

		GetDlgItem(IDC_THREE)->MoveWindow(labRc);
		moveDec -= labRc.Width() + 5;

		// ����"���µ�¼"��λ��
		GetDlgItem(IDC_RELOGIN)->GetClientRect(&labRc);

		labRc.left = moveDec - labRc.Width();
		labRc.right = moveDec;

		GetDlgItem(IDC_RELOGIN)->MoveWindow(labRc);
		moveDec -= labRc.Width() + 5;

		// �����ڶ����ָ��ߵ�λ��
		GetDlgItem(IDC_TWO)->GetClientRect(&labRc);

		labRc.left = moveDec - labRc.Width();
		labRc.right = moveDec;

		GetDlgItem(IDC_TWO)->MoveWindow(labRc);
		moveDec -= labRc.Width() + 5;

		// ����"�޸�����"��λ��
		GetDlgItem(IDC_MODIPW)->GetClientRect(&labRc);

		labRc.left = moveDec - labRc.Width();
		labRc.right = moveDec;

		GetDlgItem(IDC_MODIPW)->MoveWindow(labRc);
		moveDec -= labRc.Width() + 5;

		// ������һ���ָ��ߵ�λ��
		GetDlgItem(IDC_ONE)->GetClientRect(&labRc);

		labRc.left = moveDec - labRc.Width();
		labRc.right = moveDec;

		GetDlgItem(IDC_ONE)->MoveWindow(labRc);
		moveDec -= labRc.Width() + 5;

		// ������ӭ��Ϣ�ؼ���λ��
		GetDlgItem(IDC_WELCOMELAB)->GetClientRect(&labRc);

		labRc.left = moveDec - labRc.Width();
		labRc.right = moveDec;

		GetDlgItem(IDC_WELCOMELAB)->MoveWindow(labRc);
	}
}

void CCarManageView::OnModipw() 
{
	// TODO: Add your control notification handler code here
}

void CCarManageView::OnRelogin() 
{
	// TODO: Add your control notification handler code here
	
}

void CCarManageView::OnExit() 
{
	// TODO: Add your control notification handler code here
	
}
