// MapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CarManage.h"
#include "MapDlg.h"
#include "GpsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapDlg dialog


CMapDlg::CMapDlg(CString &purviewCoding, CWnd* pParent /*=NULL*/)
	: CDialog(CMapDlg::IDD, pParent), purviewCoding(purviewCoding)
{
	//{{AFX_DATA_INIT(CMapDlg)
	//}}AFX_DATA_INIT
}


void CMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMapDlg)
	DDX_Control(pDX, IDC_EXPLORER1, m_web);
	DDX_Control(pDX, IDC_TAB1, m_tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMapDlg, CDialog)
	//{{AFX_MSG_MAP(CMapDlg)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapDlg message handlers
BOOL CMapDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// Get Client Rect for the size change
	this->SetWindowText(_T("地图"));

	gpsDlg = NULL;
	this->ModuleSetup(purviewCoding);

	CRect rect;
	GetClientRect(&rect);
	old.x = rect.right - rect.left;
	old.y = rect.bottom - rect.top;

	// Set the Web Page
	CHAR		szPath[MAX_PATH];
	memset(szPath, 0, sizeof(CHAR)*MAX_PATH);
	GetCurrentDirectory(MAX_PATH, szPath); // 获取当前程序的路径
	StrCat(szPath, _T("\\sogoumap.html"));

	// 判断地图文件是否存在
	CFileStatus   status;
	if (!CFile::GetStatus(szPath, status)) {
		MessageBox("地图HTML文件不存在,地图将会无法显示,请检查",
					"提示信息",
					MB_ICONWARNING);

		return FALSE;
	}

	m_web.Navigate(szPath, NULL, NULL, NULL, NULL);

	// Init Web Page argument
	CComPtr<IDispatch> spDisp = m_web.GetDocument();
	m_webPage.SetDocument(spDisp);

	return TRUE;
}

// 窗口大小发生改变时的处理函数
// 比如窗口最大化最小化，窗口还原
void CMapDlg::Resize()
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

	HWND  hwndChild=::GetWindow(m_hWnd, GW_CHILD);  //列出所有控件
	while(hwndChild)  
	{
		woc=::GetDlgCtrlID(hwndChild);//取得ID
		GetDlgItem(woc)->GetWindowRect(Rect);
		ScreenToClient(Rect);
		OldTLPoint = Rect.TopLeft();
		TLPoint.x = long(OldTLPoint.x * fsp[0]);
		TLPoint.y = long(OldTLPoint.y * fsp[1]);
		OldBRPoint = Rect.BottomRight();
		BRPoint.x = long(OldBRPoint.x * fsp[0]);
		BRPoint.y = long(OldBRPoint.y * fsp[1]);
		Rect.SetRect(TLPoint, BRPoint);
		GetDlgItem(woc)->MoveWindow(Rect, TRUE);
		hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);  
	}

	old=Newp;
}

void CMapDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	Resize();

	if (m_tab) {
		// 调用 MYTAB 的 OnSize 函数
		m_tab.OnSize(0, 0, 0);

		// 设置地图的大小随窗口大小变化而变化
		CRect rc;
		CString tmpStr;
		CStringArray args;
		
		GetDlgItem(IDC_EXPLORER1)->GetWindowRect(&rc);
		tmpStr.Format("%d", rc.Width());
		args.Add(tmpStr);
		tmpStr.Format("%d", rc.Height());
		args.Add(tmpStr);

		m_webPage.CallJScript("setMapSize", args);
		m_web.UpdateData();
	}
}

BOOL CMapDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)  
		return TRUE;
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)   
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

// CallJSFunc() : 调用JAVASCRIPT的函数
// funcName : 被调用函数的名字
// args : 传递给被调用函数的参数列表
void CMapDlg::CallJSFunc(const CString &funcName, const CStringArray &args)
{
	m_webPage.CallJScript(funcName, args);
	m_web.UpdateData();
}

// ModuleSetup() : 权限权限编码设置对应模块功能是否可用
// purviewCoding : 权限编码
void CMapDlg::ModuleSetup(CString &purviewCoding)
{
	int		codingLen = (purviewCoding.GetLength() > theApp.unit) ?
						theApp.unit : purviewCoding.GetLength();
	CString subPurviewCoding = purviewCoding.Left(codingLen);
	purviewCoding.Delete(0, codingLen);

	for (int i = 0; i < codingLen; ++i) {
		if (subPurviewCoding.GetAt(i) == '1') {
			switch (i) {
			case 0:
				gpsDlg = new CGpsDlg(purviewCoding);
				gpsDlg->Create(IDD_GPSDLG, &m_tab);
				m_tab.InsertLabel("GPS功能", gpsDlg);

				break;
			default:
				break;
			}
		}
	}
}

CWebBrowser2 &CMapDlg::GetWebBrowser()
{
	return this->m_web;
}

// GetCarInfo() : 根据ID获取车辆的详细信息
// targetId : 车辆的ID
void CMapDlg::GetCarInfo(const int targetId)
{
	CStringArray args;
	targetOpt.QueryTargetInfo(targetId, args);

	m_webPage.CallJScript("setCarInfo", args);
	m_web.UpdateData();
}

void CMapDlg::CreateTargetMarker() 
{
	// TODO: Add your message handler code here and/or call default
	std::vector<TargetInfo> targetLst;
	// 查询用户所管理的所有车辆，第一个参数为用户ID
	// 现默认为0，表示查询所有车辆
	targetOpt.QueryTargetByUser(0, targetLst);

	int nCount		= targetLst.size();
	CStringArray	res;
	CString			tmpStr;
	TargetInfo		targetInfo;
	for (int i = 0; i < nCount; ++i) {
		res.RemoveAll();

		targetInfo = targetLst.at(i);

		tmpStr.Format("%d", targetInfo.targetID);
		res.Add(tmpStr);

		res.Add(targetInfo.targetName);

		tmpStr.Format("%f", targetInfo.longitude);
		res.Add(tmpStr);

		tmpStr.Format("%f", targetInfo.latitude);
		res.Add(tmpStr);

		res.Add(targetInfo.cargroup);

		m_webPage.CallJScript("markCarMarker", res);
		m_web.UpdateData();
	}
}

void CMapDlg::CreateTraceMarker(const int id) {
	if (gpsDlg) {
		gpsDlg->CreateTraceMarker(id);
	}
}

void CMapDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if (gpsDlg) {
		gpsDlg->ShowWindow(bShow, nStatus);
	}
}
