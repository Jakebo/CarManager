//////////////////////////////////////////////////////////////////////////
// 
// CSizingDialog V2.1
// 
// Copyright 2005 Xia Xiongjun( ÏÄÐÛ¾ü ), All Rights Reserved.
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
// SizingDialog.h 

#pragma once

#include "SizingSP.h"


//////////////////////////////////////////////////////////////////////////
// CSizingDialog dialog

class CSizingDialog : public CDialog
{
	DECLARE_SIZING_SUPPORT()

// Constructors
public:
	CSizingDialog(LPCTSTR lpszSection = NULL, LPCTSTR lpszEntry = NULL);
	explicit CSizingDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL, 
		LPCTSTR lpszSection = _T("Placement"), LPCTSTR lpszEntry = _T("MainDialog"));

// Implementation
public:
	virtual BOOL OnInitDialog();

protected:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg UINT OnNcHitTest(CPoint point);
	DECLARE_MESSAGE_MAP()

// Data
private:
	LPCTSTR			m_lpszSection;
	LPCTSTR			m_lpszEntry;
	BOOL			m_bEnableRWP;		// Enable to restore the window placement
	BOOL			m_bErase;			// Enable erase background
};

class CSizingFormView : public CFormView
{
};