// MyTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MyTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyTabCtrl

CMyTabCtrl::CMyTabCtrl()
{
	m_curSelIndex = -1;
}

CMyTabCtrl::~CMyTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CMyTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchangeTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyTabCtrl message handlers

void CMyTabCtrl::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int index = this->GetCurSel();

	m_dlgMap[m_curSelIndex]->ShowWindow(SW_HIDE);
	m_dlgMap[index]->ShowWindow(SW_SHOW);

	m_curSelIndex = index;

	*pResult = 0;
}

std::map<int, CDialog *>& CMyTabCtrl::GetDlgMap()
{
	return this->m_dlgMap;
}

void CMyTabCtrl::InsertLabel(const CString &labelName, CDialog *dlg)
{
	m_dlgCount = m_dlgMap.size();
	this->InsertItem(m_dlgCount, labelName);

	m_dlgMap[m_dlgCount] = dlg;

	if (m_curSelIndex != -1) {
		m_dlgMap[m_curSelIndex]->ShowWindow(SW_HIDE);
	}

	CRect rc;
	this->GetClientRect(rc);
	rc.top += 20;

	m_dlgMap[m_dlgCount]->MoveWindow(&rc);
	m_dlgMap[m_dlgCount]->ShowWindow(SW_SHOW);

	this->SetCurSel(m_dlgCount);
	m_curSelIndex = m_dlgCount;
}

void CMyTabCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CTabCtrl::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	std::map<int, CDialog *>::iterator it = m_dlgMap.begin();
	CRect rc;
	
	this->GetClientRect(rc);

	rc.top += 20;

	while (it != m_dlgMap.end()) {
		it->second->MoveWindow(rc);

		++it;
	}
}