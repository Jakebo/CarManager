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

	// 查询登录用户的权限编码
	CString purviewCoding;
	ro.QueryUserPurviewByName(CLogDlg::GetID(), purviewCoding);
	// 根据权限编码来设置系统的各个功能模块是否可用
	ModuleSetup(purviewCoding);

	// 设置登录信息
	(GetDlgItem(IDC_WELCOMELAB))->SetWindowText(CLogDlg::GetName() + "  欢迎你");
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
	
	// 顶级TAB   ROLEDLG MAPDLG T  GPS   
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

					m_tab.InsertLabel("车辆资料管理", m_dlgMap[CARDATADLG]);
				}
				break;
			case 1:
				if (m_dlgMap[ROLEMANAGEDLG] == NULL) {
					m_dlgMap[ROLEMANAGEDLG] = new CRoleManageDlg(purviewCoding);
					m_dlgMap[ROLEMANAGEDLG]->Create(IDD_ROLEDLG, &m_tab);
					
					m_tab.InsertLabel("权限管理", m_dlgMap[ROLEMANAGEDLG]);	
				}
				break;
			case 2:
				if (m_dlgMap[MAPDLG] == NULL) {
					mapDlg = new CMapDlg(purviewCoding);
					m_dlgMap[MAPDLG] = mapDlg;
					m_dlgMap[MAPDLG]->Create(IDD_DIALOG1, &m_tab);

					m_tab.InsertLabel("地图", m_dlgMap[MAPDLG]);
				}
				break;
			default:
				break;
			}
		}
	}

	// 如果该登录的用户拥有权限管理的权限，则显示权限树中的内容
	// 或许这段代码不应放在这里，应放在上面的代码中，现暂时放在这里
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

	// 遍历 Static 控件以调整他们的位置
	if (GetDlgItem(IDC_EXIT)) {
		CRect	labRc;
		int		moveDec;

		// 调整"退出"位置
		GetDlgItem(IDC_EXIT)->GetClientRect(&labRc); // 获取控件的客户区域

		labRc.left = rc.right - labRc.Width(); // 设置区域左右位置
		labRc.right = rc.right;

		GetDlgItem(IDC_EXIT)->MoveWindow(labRc); // 设置控件位置
		moveDec = rc.right - labRc.Width() - 5;

		// 调整第三个分隔线的位置
		GetDlgItem(IDC_THREE)->GetClientRect(&labRc);

		labRc.left = moveDec - labRc.Width();
		labRc.right = moveDec;

		GetDlgItem(IDC_THREE)->MoveWindow(labRc);
		moveDec -= labRc.Width() + 5;

		// 调整"重新登录"的位置
		GetDlgItem(IDC_RELOGIN)->GetClientRect(&labRc);

		labRc.left = moveDec - labRc.Width();
		labRc.right = moveDec;

		GetDlgItem(IDC_RELOGIN)->MoveWindow(labRc);
		moveDec -= labRc.Width() + 5;

		// 调整第二个分隔线的位置
		GetDlgItem(IDC_TWO)->GetClientRect(&labRc);

		labRc.left = moveDec - labRc.Width();
		labRc.right = moveDec;

		GetDlgItem(IDC_TWO)->MoveWindow(labRc);
		moveDec -= labRc.Width() + 5;

		// 调整"修改密码"的位置
		GetDlgItem(IDC_MODIPW)->GetClientRect(&labRc);

		labRc.left = moveDec - labRc.Width();
		labRc.right = moveDec;

		GetDlgItem(IDC_MODIPW)->MoveWindow(labRc);
		moveDec -= labRc.Width() + 5;

		// 调整第一个分隔线的位置
		GetDlgItem(IDC_ONE)->GetClientRect(&labRc);

		labRc.left = moveDec - labRc.Width();
		labRc.right = moveDec;

		GetDlgItem(IDC_ONE)->MoveWindow(labRc);
		moveDec -= labRc.Width() + 5;

		// 调整欢迎信息控件的位置
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
