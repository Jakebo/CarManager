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

	// ȡ�ó����ı���;�γ��
	targetOpt.QueryTargetCargroup(targetID, cargroup);
	targetOpt.QueryTargetPosition(targetID, longitude, latitude);

	// ���ô��ڱ���
	this->SetWindowText(cargroup + ":" + targetName + "  ��̬����"); 

	// ��ȡ�������ݷŵ������б���
	tmp.Format("%d", targetID);
	args.Add(tmp);
	args.Add((cargroup + ":" + targetName));
	tmp.Format("%f", longitude);
	args.Add(tmp);
	tmp.Format("%f", latitude);
	args.Add(tmp);

	// ����JS�������ڵ�ͼ������־
	m_webPage.CallJScript("setMarkInfo", args);
	m_traceWeb.UpdateData();
}

// ���ڴ�С�����ı�ʱ�Ĵ�����
// ���細�������С�������ڻ�ԭ
void CTraceDlg::Resize()
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

BOOL CTraceDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rect;
	GetClientRect(&rect);
	old.x = rect.right - rect.left;
	old.y = rect.bottom - rect.top;

	nElapse = 1500; // ��ʼ��ʱ����ʱ����Ϊ1500ms

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
		// ����web�ؼ���ҳ��
		m_traceWeb.Navigate(szPath, NULL, NULL, NULL, NULL);
		// ��ʼ��WebPage��Document
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

		MessageBox("�켣���ٵĵ�ͼHTML�ļ�������,��ͼ�޷���ʾ\n��ȷ�ϸó���װĿ¼�����track.html�ļ�",
					"��ʾ��Ϣ",
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
			// ȡ�ó�����ǰ�ľ�γ��
			targetOpt.QueryTargetPosition(targetID, longitude, latitude);

			// ��ȡ���ľ�γ�ȷŵ�������
			tmp.Format("%f", longitude);
			args.Add(tmp);
			tmp.Format("%f", latitude);
			args.Add(tmp);

			// ����JS���������ٲ������߹���·��
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
