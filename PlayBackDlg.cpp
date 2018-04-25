// PlayBackDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CarManage.h"
#include "PlayBackDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HWND g_hWnd;

CString CPlayBackDlg::startTime;
CString CPlayBackDlg::endTime;
/////////////////////////////////////////////////////////////////////////////
// CPlayBackDlg dialog

CPlayBackDlg::CPlayBackDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlayBackDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlayBackDlg)
	//}}AFX_DATA_INIT
}

// CPlayBackDlg : 构造函数
// targetID : Target 的ID
// targetName : Target 的名字
CPlayBackDlg::CPlayBackDlg(const int targetID,
						   const CString targetName,
						   CWnd* pParent /*=NULL*/)
	: CDialog(CPlayBackDlg::IDD, pParent),
	targetID(targetID), targetName(targetName)
{
}

void CPlayBackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlayBackDlg)
	DDX_Control(pDX, IDC_BUTTON4, m_selectTime);
	DDX_Control(pDX, IDC_STATIC13, m_msgLab);
	DDX_Control(pDX, IDC_STATIC11, m_eSecLab);
	DDX_Control(pDX, IDC_STATIC6, m_sSecLab);
	DDX_Control(pDX, IDC_COMBO13, m_esec);
	DDX_Control(pDX, IDC_COMBO10, m_ssec);
	DDX_Control(pDX, IDC_BUTTON1, m_startPB);
	DDX_Control(pDX, IDC_PROGRESS1, m_dlPro);
	DDX_Control(pDX, IDC_BUTTON3, m_replayBtn);
	DDX_Control(pDX, IDC_STATICE2, m_endLab);
	DDX_Control(pDX, IDC_STATIC10, m_eMinLab);
	DDX_Control(pDX, IDC_STATIC9, m_eHourLab);
	DDX_Control(pDX, IDC_STATIC5, m_sMinLab);
	DDX_Control(pDX, IDC_STATIC4, m_sHourLab);
	DDX_Control(pDX, IDC_STATICS1, m_startLab);
	DDX_Control(pDX, IDCANCEL, m_cancelbtn);
	DDX_Control(pDX, IDOK, m_okbtn);
	DDX_Control(pDX, IDC_COMBO12, m_emin);
	DDX_Control(pDX, IDC_COMBO8, m_ehour);
	DDX_Control(pDX, IDC_COMBO9, m_smin);
	DDX_Control(pDX, IDC_COMBO4, m_shour);
	DDX_Control(pDX, IDC_EXPLORER1, m_playbackWeb);
	DDX_Control(pDX, IDC_DTPICKER1, m_sPicker);
	DDX_Control(pDX, IDC_DTPICKER2, m_ePicker);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlayBackDlg, CDialog)
	//{{AFX_MSG_MAP(CPlayBackDlg)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON3, OnRePlay)
	ON_BN_CLICKED(IDC_BUTTON1, OnStartPB)
	ON_BN_CLICKED(IDC_BUTTON4, OnSelectTime)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayBackDlg message handlers
BOOL CPlayBackDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rect;
	GetClientRect(&rect);
	old.x = rect.right - rect.left;
	old.y = rect.bottom - rect.top;

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
		m_playbackWeb.Navigate(szPath, NULL, NULL, NULL, NULL);

		// 初始化WebPage的Document
		CComPtr<IDispatch> spDisp = m_playbackWeb.GetDocument();
		m_webPage.SetDocument(spDisp);
	}

	// 初始化进度条
	m_dlPro.SetRange(0, 100);
	m_dlPro.SetStep(100);

	showMap = FALSE;

	SelectTime (); // 设置控件是否可见

	interval = 1500;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CPlayBackDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	return 0;
}

void CPlayBackDlg::OnOK() 
{
	// TODO: Add extra validation here
	VARIANT year, month, day;
	int select;
	CString hour, min, sec;
	
	year.vt		= VT_I4;
	month.vt	= VT_I4;
	day.vt		= VT_I4;

	// 获取时间段的起始时间
	year		= m_sPicker.GetYear();
	month		= m_sPicker.GetMonth();
	day			= m_sPicker.GetDay();

	select = m_shour.GetCurSel();
	m_shour.GetLBText(select, hour);
	select = m_smin.GetCurSel();
	m_smin.GetLBText(select, min);
	select = m_ssec.GetCurSel();
	m_ssec.GetLBText(select, sec);

	startTime.Format("%d-%d-%d ", year.lVal, month.lVal, day.lVal);
	startTime = startTime + hour + ":" + min + ":" + sec;

	CTime   time1(year.lVal, month.lVal, day.lVal,
				  atoi(hour), atoi(min), atoi(sec)); 
	// 获取时间段的结束时间
	year		= m_ePicker.GetYear();
	month		= m_ePicker.GetMonth();
	day			= m_ePicker.GetDay();

	select = m_ehour.GetCurSel();
	m_ehour.GetLBText(select, hour);
	select = m_emin.GetCurSel();
	m_emin.GetLBText(select, min);
	select = m_esec.GetCurSel();
	m_esec.GetLBText(select, sec);

	endTime.Format("%d-%d-%d ",year.lVal, month.lVal, day.lVal);
	endTime = endTime + hour + ":" + min + ":" + sec;

	CTime   time2(year.lVal, month.lVal, day.lVal,
				  atoi(hour), atoi(min), atoi(sec));

	if (time2 < time1) {
		MessageBox("起始时间不应大于结束时间",
				   "提示信息",
				   MB_ICONWARNING);
		m_dlPro.SetPos(0);
		return;
	}

	DownloadLocus(); // 下载车辆轨迹
	this->m_startPB.EnableWindow(TRUE);
}

// DownloadLocus() : 下载当前车辆指定时间段的轨迹
void CPlayBackDlg::DownloadLocus()
{
	// 下载指定时间段的车辆轨迹
	targetOpt.QueryTargetLocus(targetID, startTime, endTime, pRst);

	m_dlPro.SetPos(100);
}

void CPlayBackDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	::SendMessage(g_hWnd, WM_DELETEPBDLG, targetID, 0);
}

void CPlayBackDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	::SendMessage(g_hWnd, WM_DELETEPBDLG, targetID, 0);
}

void CPlayBackDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	MoveWebWindow();
}

// 窗口大小发生改变时的处理函数
// 比如窗口最大化最小化，窗口还原
void CPlayBackDlg::Resize()
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

void CPlayBackDlg::InitTime()
{
	CTime now = CTime::GetCurrentTime();
	CString str;

	for (int i = 0; i < 60; ++i) {
		str.Format("%d", i);
		m_smin.AddString(str);
		m_emin.AddString(str);
		m_ssec.AddString(str);
		m_esec.AddString(str);
		if (i < 24) {
			m_shour.AddString(str);
			m_ehour.AddString(str);
		}
	}

	m_shour.SetCurSel(0);
	m_smin.SetCurSel(0);
	m_ehour.SetCurSel(0);
	m_emin.SetCurSel(0);
	m_ssec.SetCurSel(0);
	m_esec.SetCurSel(0);
}

// SelectTime() ：选择轨迹回放的时间段
void CPlayBackDlg::SelectTime() {
	this->m_replayBtn.ShowWindow(SW_HIDE);
	this->m_selectTime.ShowWindow(SW_HIDE);

	this->SetWindowText("选轨迹回放的时间段");
	this->m_startPB.EnableWindow(FALSE);

	InitTime();
}

// ShowPBMap() : 显示轨迹回放的地图
void CPlayBackDlg::ShowPBMap ()
{
	this->m_sPicker.ShowWindow(SW_HIDE);
	this->m_ePicker.ShowWindow(SW_HIDE);
	this->m_shour.ShowWindow(SW_HIDE);
	this->m_ehour.ShowWindow(SW_HIDE);
	this->m_smin.ShowWindow(SW_HIDE);
	this->m_emin.ShowWindow(SW_HIDE);
	this->m_ssec.ShowWindow(SW_HIDE);
	this->m_esec.ShowWindow(SW_HIDE);
	this->m_startLab.ShowWindow(SW_HIDE);
	this->m_endLab.ShowWindow(SW_HIDE);
	this->m_sHourLab.ShowWindow(SW_HIDE);
	this->m_sMinLab.ShowWindow(SW_HIDE);
	this->m_eHourLab.ShowWindow(SW_HIDE);
	this->m_eMinLab.ShowWindow(SW_HIDE);
	this->m_sSecLab.ShowWindow(SW_HIDE);
	this->m_eSecLab.ShowWindow(SW_HIDE);
	this->m_dlPro.ShowWindow(SW_HIDE);
	this->m_startPB.ShowWindow(SW_HIDE);
	this->m_okbtn.ShowWindow(SW_HIDE);
	this->m_cancelbtn.ShowWindow(SW_HIDE);
	this->m_msgLab.ShowWindow(SW_HIDE);
} 

void CPlayBackDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent) {
	case TIMEID2: // 定时器2：定时向Web发送数据
		{
			WriteDataToWeb();
			break;
		}
	default:
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

// InitPlayback() : 初始化轨迹回放所需的各个参数
void CPlayBackDlg::InitPlayback()
{
	
	CString cargroup;
	CString title;

	// 查询车辆的编码
	targetOpt.QueryTargetCargroup(targetID, cargroup);
	// 设置窗口标题
	title = cargroup + ":" + targetName + "  轨迹回放";
	this->SetWindowText(title);

	// 在地图上做被跟踪车辆的起始标志
	if (!pRst->adoEOF) {
		CString tID;
		float i, j;

		args.RemoveAll(); // 清空参数列表里的所有参数

		tID.Format("%d", targetID);
		args.Add(tID);
		args.Add(targetName);
		GetCollect(pRst, "longitude", i);
		GetCollect(pRst, "latitude", j);
		longitude.Format("%f", i);
		args.Add(longitude);
		latitude.Format("%f", j);
		args.Add(latitude);

		m_webPage.CallJScript("setMarkInfo", args);
		m_playbackWeb.UpdateData();

		// 显示地图
		showMap = TRUE;
		ShowPBMap();
		MoveWebWindow();

		SetTimer(TIMEID2, 1500, 0);
	} else {
		m_startPB.EnableWindow(FALSE);
		m_dlPro.SetPos(0);
		MessageBox("所选择的时间段里没有轨迹记录",
				   "提示信息",
				   MB_ICONWARNING);
	}
}

// 将下载的车辆轨迹发送到Web页面显示
void CPlayBackDlg::WriteDataToWeb()
{
	if (pRst->adoEOF) { // 回放结束
		showMap = FALSE; // 隐藏地图
		MoveWebWindow();
		m_msgLab.SetWindowText("轨迹回放结束");
		m_msgLab.ShowWindow(SW_SHOW);
		m_replayBtn.ShowWindow(SW_SHOW);
		m_selectTime.ShowWindow(SW_SHOW);
		m_endLab.SetWindowText("轨迹回放结束");

		KillTimer(TIMEID2); // 关闭定时器2，不再向web发送数据
		return;
	}

	args.RemoveAll();

	float lon, lat; // 经纬度
	float sp; // 速度

	args.Add(targetName); // JS的第一个参数：name
	//获取时间并转换为字符串
	variant_t vt;
	vt = pRst->GetCollect("time");
	if (vt.vt != VT_NULL) {
		vt.ChangeType(VT_BSTR); // 转换为字符串
		time = vt.bstrVal;
	}
	args.Add(time); // 第二个参数：time
	GetCollect(pRst, "speed", sp);
	speed.Format("%f", sp);
	args.Add(speed); // 第三个参数：speed
	GetCollect(pRst, "longitude", lon);
	GetCollect(pRst, "latitude", lat);
	longitude.Format("%f", lon);
	args.Add(longitude); // 第四个参数:longitude
	latitude.Format("%f", lat);
	args.Add(latitude); // 第五个参数：latitude

	// 向web页面写数据
	m_webPage.CallJScript("moveTraceMarkerPlayback", args);
	m_playbackWeb.UpdateData();
	// 移动到下一条
	pRst->MoveNext();
}

BOOL CPlayBackDlg::GetCollect(_RecordsetPtr &pRec, LPCTSTR Name, float &res)
{
	variant_t vt;

	try{
		vt = pRec->GetCollect(Name);

		if (vt.vt != VT_NULL) {
			vt.ChangeType(VT_R4);
			res = vt.fltVal;
		} else {
			return FALSE;
		}
	}catch (_com_error e) {
		TRACE (e.Description());
	}

	return TRUE;
}

void CPlayBackDlg::OnRePlay()
{
	// TODO: Add your control notification handler code here
	m_endLab.ShowWindow(SW_HIDE);
	m_replayBtn.ShowWindow(SW_HIDE);

	m_webPage.CallJScript("replay", NULL);
	m_playbackWeb.UpdateData();

	showMap = TRUE;
	MoveWebWindow();

	pRst->MoveFirst();
	SetTimer(TIMEID2, interval, 0);
}

BOOL CPlayBackDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)  
		return TRUE;
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) {
		::SendMessage(g_hWnd, WM_DELETEPBDLG, targetID, 0);
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CPlayBackDlg::OnStartPB() 
{
	// TODO: Add your control notification handler code here
	if (!hasMapFile) {
		MessageBox("轨迹跟踪的地图HTML文件不存在,地图无法显示\n请确认该程序安装目录里存在track.html文件",
					"提示信息",
					MB_ICONWARNING);
		return;
	}

	InitPlayback(); // 初始化回放
}

// MoveWebWindow() : 改变浏览器窗口的位置以显示或隐藏的效果
void CPlayBackDlg::MoveWebWindow()
{
	// 显示地图，因为activeX控件的显示与隐藏的问题，不得不出此下策
	// 以后有好方法再作打算
	if (m_playbackWeb) {
		CRect rc(0, 0, 0, 0); // 如果不打算显示地图，则全设为0
		if (showMap) {
			this->GetClientRect(&rc);
		}
		this->m_playbackWeb.MoveWindow(rc);
	}
}


void CPlayBackDlg::OnSelectTime() 
{
	// TODO: Add your control notification handler code here
	showMap = FALSE;
	MoveWebWindow(); // 隐藏地图

	this->m_sPicker.ShowWindow(SW_SHOW);
	this->m_ePicker.ShowWindow(SW_SHOW);
	this->m_shour.ShowWindow(SW_SHOW);
	this->m_ehour.ShowWindow(SW_SHOW);
	this->m_smin.ShowWindow(SW_SHOW);
	this->m_emin.ShowWindow(SW_SHOW);
	this->m_ssec.ShowWindow(SW_SHOW);
	this->m_esec.ShowWindow(SW_SHOW);
	this->m_startLab.ShowWindow(SW_SHOW);
	this->m_endLab.ShowWindow(SW_SHOW);
	this->m_sHourLab.ShowWindow(SW_SHOW);
	this->m_sMinLab.ShowWindow(SW_SHOW);
	this->m_eHourLab.ShowWindow(SW_SHOW);
	this->m_eMinLab.ShowWindow(SW_SHOW);
	this->m_sSecLab.ShowWindow(SW_SHOW);
	this->m_eSecLab.ShowWindow(SW_SHOW);
	this->m_dlPro.ShowWindow(SW_SHOW);
	this->m_okbtn.ShowWindow(SW_SHOW);
	this->m_startPB.ShowWindow(SW_SHOW);
	this->m_startPB.EnableWindow(FALSE);
	this->m_cancelbtn.ShowWindow(SW_SHOW);
	this->m_msgLab.ShowWindow(SW_HIDE);
	this->m_dlPro.SetPos(0);

	SelectTime();
}
