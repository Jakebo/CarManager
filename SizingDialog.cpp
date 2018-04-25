//////////////////////////////////////////////////////////////////////////
// 
// CSizingDialog V2.1
// 
// Copyright 2005 Xia Xiongjun( 夏雄军 ), All Rights Reserved.
//
// E-mail: xj-14@163.com
//
// This source file can be copyed, modified, redistributed  by any means
// PROVIDING that this notice and the author's name and all copyright 
// notices remain intact, and PROVIDING it is NOT sold for profit without 
// the author's expressed written consent. The author accepts no 
// liability for any damage/loss of business that this product may cause.
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// SizingDialog.cpp :  implementation file

#include "stdafx.h"
#include "SizingDialog.h"
#include ".\SizingDialog.h"

#define ID_TIMER				1


//////////////////////////////////////////////////////////////////////////
// CSizingDialog dialog

IMPLEMENT_SIZING_SUPPORT(CSizingDialog)

CSizingDialog::CSizingDialog(
	LPCTSTR lpszSection /*= NULL*/, LPCTSTR lpszEntry /*= NULL*/)
	: m_lpszSection(lpszSection)
	, m_lpszEntry(lpszEntry)
	, m_bEnableRWP(TRUE)
	, m_bErase(TRUE)
{
	CDialog::CDialog();
}

CSizingDialog::CSizingDialog(
	UINT nIDTemplate, CWnd* pParentWnd /*= NULL*/, 
	LPCTSTR lpszSection /*= _T("Placement")*/, 
	LPCTSTR lpszEntry /*= _T("MainDialog")*/)
	: CDialog(nIDTemplate, pParentWnd)
	, m_lpszSection(lpszSection)
	, m_lpszEntry(lpszEntry)
	, m_bEnableRWP(TRUE)
	, m_bErase(TRUE)
{
}

BEGIN_MESSAGE_MAP(CSizingDialog, CDialog)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

// CSizingDialog message handlers
BOOL CSizingDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// 以下这段代码可解决无法设置对话框无边框、大小不可变的问题
	DWORD dwStyle = GetStyle();//获取旧样式  
    DWORD dwNewStyle = WS_OVERLAPPED | WS_VISIBLE | WS_SYSMENU | WS_CHILD |
		WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;  
    dwNewStyle&=dwStyle;//按位与将旧样式去掉  
    SetWindowLong(this->m_hWnd, GWL_STYLE, dwNewStyle);//设置成新的样式  
    DWORD dwExStyle = GetExStyle();//获取旧扩展样式  
    DWORD dwNewExStyle = WS_EX_LEFT |
		WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR;  
    dwNewExStyle&=dwExStyle;//按位与将旧扩展样式去掉  
    SetWindowLong(this->m_hWnd, GWL_EXSTYLE, dwNewExStyle);//设置新的扩展样式  
	//告诉windows：我的样式改变了，窗口位置和大小保持原来不变！
    SetWindowPos(NULL, 0, 0, 0, 0,
		SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CSizingDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	::SetWindowLong(m_hWnd, GWL_STYLE, 
		::GetWindowLong(m_hWnd, GWL_STYLE) | WS_THICKFRAME);

	Initialize(m_hWnd);

	return 0;
}

void CSizingDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (m_bEnableRWP == FALSE)
		return;

	// The following code will be run only once
	m_bEnableRWP = FALSE;

	// Restore window placement
	if (m_lpszSection == NULL || m_lpszEntry == NULL)
		return;

	WINDOWPLACEMENT* pwp;
	UINT nBytes;
	if (::AfxGetApp()->GetProfileBinary(m_lpszSection, m_lpszEntry, 
		(LPBYTE *)&pwp, &nBytes))
	{
		if (pwp->showCmd == SW_SHOWMINIMIZED)
			pwp->showCmd = pwp->flags + 1;
		if (pwp->showCmd == SW_SHOWMAXIMIZED)
			m_bErase = FALSE;
		SetWindowPlacement(pwp);

		MYDELETEARRAY(pwp);
	}
}

void CSizingDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	m_objSizingSP.ResizeControls();
	if (IsWindowVisible())
		::SetTimer(m_hWnd, ID_TIMER, 10, NULL);
}

void CSizingDialog::OnTimer(UINT nIDEvent)
{
	::KillTimer(m_hWnd, ID_TIMER);
	m_objSizingSP.UpdateGBoxFPic();

	CDialog::OnTimer(nIDEvent);
}

void CSizingDialog::OnPaint() 
{
	CDialog::OnPaint();
}

BOOL CSizingDialog::OnEraseBkgnd(CDC* pDC)
{
	//return CDialog::OnEraseBkgnd(pDC);

	if (m_bErase != TRUE)
	{
		m_bErase = TRUE;
		return FALSE;
	}

	m_objSizingSP.EraseBkgnd(pDC->m_hDC);
	return TRUE;
}

void CSizingDialog::OnDestroy()
{
	CDialog::OnDestroy();

	// Save window placement
	if (m_lpszSection == NULL || m_lpszEntry == NULL)
		return;

	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	::AfxGetApp()->WriteProfileBinary(m_lpszSection, m_lpszEntry, 
		(LPBYTE)&wp, sizeof(wp));
}

UINT CSizingDialog::OnNcHitTest(CPoint point)
{
	// move the window when clicked
	UINT nHitTest = CDialog::OnNcHitTest(point);
	if(nHitTest == HTCLIENT)
		nHitTest = HTCAPTION;
	return nHitTest;
}

