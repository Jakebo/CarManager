// TraceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CarManage.h"
#include "TraceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HWND g_hWnd;
/////////////////////////////////////////////////////////////////////////////
// CTraceDlg dialog


CTraceDlg::CTraceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTraceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTraceDlg)
	//}}AFX_DATA_INIT
}

CTraceDlg::CTraceDlg(int targetID,
					 CString targetName,
					 CWnd* pParent /*=NULL*/)
	: CDialog(CTraceDlg::IDD, pParent)
{
	this->targetID = targetID;
	this->targetName = targetName;
}

void CTraceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTraceDlg)
	DDX_Control(pDX, IDC_EXPLORER1, m_traceWeb);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTraceDlg, CDialog)
	//{{AFX_MSG_MAP(CTraceDlg)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTraceDlg message handlers

void CTraceDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	Resize();
	if (m_traceWeb) {
		CRect rc;
		this->GetClientRect(&rc);
		m_traceWeb.MoveWindow(rc);
	}
}

void CTraceDlg::CreateTraceMarker()
{
	CStringArray args;
	CString cargroup;
	CString tmp;
	float longitude;
	float latitude;

	// 取得车辆的编码和经纬度
	targetOpt.QueryTargetCargroup(targetID, cargroup);
	targetOpt.QueryTargetPosition(targetID, longitude, latitude);

	// 设置窗口标题
	this->SetWindowText(cargroup + ":" + targetName + "  动态跟踪"); 

	// 将取到的数据放到参数列表中
	tmp.Format("%d", targetID);
	args.Add(tmp);
	args.Add((cargroup + ":" + targetName));
	tmp.Format("%f", longitude);
	args.Add(tmp);
	tmp.Format("%f", latitude);
	args.Add(tmp);

	// 调用JS函数，在地图上做标志
	m_webPage.CallJScript("setMarkInfo", args);
	m_traceWeb.UpdateData();
}

// 窗口大小发生改变时的处理函数
// 比如窗口最大化最小化，窗口还原
void CTraceDlg::Resize()
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

BOOL CTraceDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rect;
	GetClientRect(&rect);
	old.x = rect.right - rect.left;
	old.y = rect.bottom - rect.top;

	nElapse = 1500; // 初始定时器的时间间隔为1500ms

	// 获取当前程序的路径并加上地图文件
	CHAR		szPath[MAX_PATH];
	memset(szPath, 0, sizeof(CHAR)*MAX_PATH);
	GetCurrentDirectory(MAX_PATH, szPath);
	StrCat(szPath, _T("\\tracemap.html"));

	// 判断地图文件是否存在
	CFileStatus   status;
	hasMapFile = TRUE;
	if (!CFile::GetStatus(szPath, status)) {
		hasMapFile = FALSE;
	} else {
		// 设置web控件的页面
		m_traceWeb.Navigate(szPath, NULL, NULL, NULL, NULL);
		// 初始化WebPage的Document
		CComPtr<IDispatch> spDisp = m_traceWeb.GetDocument();
		m_webPage.SetDocument(spDisp);
	}

	SetTimer(TIMEID2, nElapse, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CTraceDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	return 0;
}

void CTraceDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (!hasMapFile) {
		KillTimer(TIMEID2);

		MessageBox("轨迹跟踪的地图HTML文件不存在,地图无法显示\n请确认该程序安装目录里存在track.html文件",
					"提示信息",
					MB_ICONWARNING);

		return;
	}

	switch (nIDEvent) {
	case TIMEID2:
		{
			CStringArray args;
			CString tmp;
			float longitude;
			float latitude;

			tmp.Format("%d", targetID);
			args.Add(tmp);
			// 取得车辆当前的经纬度
			targetOpt.QueryTargetPosition(targetID, longitude, latitude);

			// 将取到的经纬度放到数组中
			tmp.Format("%f", longitude);
			args.Add(tmp);
			tmp.Format("%f", latitude);
			args.Add(tmp);

			// 调用JS函数，跟踪并绘制走过的路线
			m_webPage.CallJScript("moveTraceMarker", args);
			m_traceWeb.UpdateData();

			break;
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CTraceDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	::SendMessage(g_hWnd, WM_DELETETRACEDLG, targetID, 0);
}

BOOL CTraceDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)  
		return TRUE;
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) {
		::SendMessage(g_hWnd, WM_DELETETRACEDLG, targetID, 0);
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
} 
