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

// CPlayBackDlg : ���캯��
// targetID : Target ��ID
// targetName : Target ������
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

	// ��ȡ��ǰ�����·�������ϵ�ͼ�ļ�
	CHAR		szPath[MAX_PATH];
	memset(szPath, 0, sizeof(CHAR)*MAX_PATH);
	GetCurrentDirectory(MAX_PATH, szPath);
	StrCat(szPath, _T("\\tracemap.html"));
	

	// �жϵ�ͼ�ļ��Ƿ����
	CFileStatus   status;
	hasMapFile = TRUE;
	if (!CFile::GetStatus(szPath, status)) {
		hasMapFile = FALSE;
	} else {
		m_playbackWeb.Navigate(szPath, NULL, NULL, NULL, NULL);

		// ��ʼ��WebPage��Document
		CComPtr<IDispatch> spDisp = m_playbackWeb.GetDocument();
		m_webPage.SetDocument(spDisp);
	}

	// ��ʼ��������
	m_dlPro.SetRange(0, 100);
	m_dlPro.SetStep(100);

	showMap = FALSE;

	SelectTime (); // ���ÿؼ��Ƿ�ɼ�

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

	// ��ȡʱ��ε���ʼʱ��
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
	// ��ȡʱ��εĽ���ʱ��
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
		MessageBox("��ʼʱ�䲻Ӧ���ڽ���ʱ��",
				   "��ʾ��Ϣ",
				   MB_ICONWARNING);
		m_dlPro.SetPos(0);
		return;
	}

	DownloadLocus(); // ���س����켣
	this->m_startPB.EnableWindow(TRUE);
}

// DownloadLocus() : ���ص�ǰ����ָ��ʱ��εĹ켣
void CPlayBackDlg::DownloadLocus()
{
	// ����ָ��ʱ��εĳ����켣
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

// ���ڴ�С�����ı�ʱ�Ĵ�����
// ���細�������С�������ڻ�ԭ
void CPlayBackDlg::Resize()
{
	float fsp[2];
	POINT Newp; //��ȡ���ڶԻ���Ĵ�С
	CRect recta;

	GetClientRect(&recta);     //ȡ�ͻ�����С
	Newp.x=recta.right-recta.left;
	Newp.y=recta.bottom-recta.top;
	fsp[0]=(float)Newp.x/old.x;
	fsp[1]=(float)Newp.y/old.y;

	CRect Rect;
	int woc;
	CPoint OldTLPoint,TLPoint; //���Ͻ�
	CPoint OldBRPoint,BRPoint; //���½�

	HWND  hwndChild=::GetWindow(m_hWnd, GW_CHILD);  //�г����пؼ�
	while(hwndChild)  
	{  
		woc=::GetDlgCtrlID(hwndChild);//ȡ��ID
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

// SelectTime() ��ѡ��켣�طŵ�ʱ���
void CPlayBackDlg::SelectTime() {
	this->m_replayBtn.ShowWindow(SW_HIDE);
	this->m_selectTime.ShowWindow(SW_HIDE);

	this->SetWindowText("ѡ�켣�طŵ�ʱ���");
	this->m_startPB.EnableWindow(FALSE);

	InitTime();
}

// ShowPBMap() : ��ʾ�켣�طŵĵ�ͼ
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
	case TIMEID2: // ��ʱ��2����ʱ��Web��������
		{
			WriteDataToWeb();
			break;
		}
	default:
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

// InitPlayback() : ��ʼ���켣�ط�����ĸ�������
void CPlayBackDlg::InitPlayback()
{
	
	CString cargroup;
	CString title;

	// ��ѯ�����ı���
	targetOpt.QueryTargetCargroup(targetID, cargroup);
	// ���ô��ڱ���
	title = cargroup + ":" + targetName + "  �켣�ط�";
	this->SetWindowText(title);

	// �ڵ�ͼ���������ٳ�������ʼ��־
	if (!pRst->adoEOF) {
		CString tID;
		float i, j;

		args.RemoveAll(); // ��ղ����б�������в���

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

		// ��ʾ��ͼ
		showMap = TRUE;
		ShowPBMap();
		MoveWebWindow();

		SetTimer(TIMEID2, 1500, 0);
	} else {
		m_startPB.EnableWindow(FALSE);
		m_dlPro.SetPos(0);
		MessageBox("��ѡ���ʱ�����û�й켣��¼",
				   "��ʾ��Ϣ",
				   MB_ICONWARNING);
	}
}

// �����صĳ����켣���͵�Webҳ����ʾ
void CPlayBackDlg::WriteDataToWeb()
{
	if (pRst->adoEOF) { // �طŽ���
		showMap = FALSE; // ���ص�ͼ
		MoveWebWindow();
		m_msgLab.SetWindowText("�켣�طŽ���");
		m_msgLab.ShowWindow(SW_SHOW);
		m_replayBtn.ShowWindow(SW_SHOW);
		m_selectTime.ShowWindow(SW_SHOW);
		m_endLab.SetWindowText("�켣�طŽ���");

		KillTimer(TIMEID2); // �رն�ʱ��2��������web��������
		return;
	}

	args.RemoveAll();

	float lon, lat; // ��γ��
	float sp; // �ٶ�

	args.Add(targetName); // JS�ĵ�һ��������name
	//��ȡʱ�䲢ת��Ϊ�ַ���
	variant_t vt;
	vt = pRst->GetCollect("time");
	if (vt.vt != VT_NULL) {
		vt.ChangeType(VT_BSTR); // ת��Ϊ�ַ���
		time = vt.bstrVal;
	}
	args.Add(time); // �ڶ���������time
	GetCollect(pRst, "speed", sp);
	speed.Format("%f", sp);
	args.Add(speed); // ������������speed
	GetCollect(pRst, "longitude", lon);
	GetCollect(pRst, "latitude", lat);
	longitude.Format("%f", lon);
	args.Add(longitude); // ���ĸ�����:longitude
	latitude.Format("%f", lat);
	args.Add(latitude); // �����������latitude

	// ��webҳ��д����
	m_webPage.CallJScript("moveTraceMarkerPlayback", args);
	m_playbackWeb.UpdateData();
	// �ƶ�����һ��
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
		MessageBox("�켣���ٵĵ�ͼHTML�ļ�������,��ͼ�޷���ʾ\n��ȷ�ϸó���װĿ¼�����track.html�ļ�",
					"��ʾ��Ϣ",
					MB_ICONWARNING);
		return;
	}

	InitPlayback(); // ��ʼ���ط�
}

// MoveWebWindow() : �ı���������ڵ�λ������ʾ�����ص�Ч��
void CPlayBackDlg::MoveWebWindow()
{
	// ��ʾ��ͼ����ΪactiveX�ؼ�����ʾ�����ص����⣬���ò������²�
	// �Ժ��к÷�����������
	if (m_playbackWeb) {
		CRect rc(0, 0, 0, 0); // �����������ʾ��ͼ����ȫ��Ϊ0
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
	MoveWebWindow(); // ���ص�ͼ

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
