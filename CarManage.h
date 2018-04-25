// CarManage.h : main header file for the CARMANAGE application
//

#if !defined(AFX_CARMANAGE_H__0EE0D8C0_D500_4718_93CA_7B7160FE3146__INCLUDED_)
#define AFX_CARMANAGE_H__0EE0D8C0_D500_4718_93CA_7B7160FE3146__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include <map>

#include "resource.h"       // main symbols

#define WM_DELETETRACEDLG				WM_USER+1 // 关闭动态跟踪窗口的消息
#define WM_DELETEPBDLG					WM_USER+2 // 关闭轨迹回放窗口的消息
#define WM_DELETECARINFODLG				WM_USER+3 // 关闭车辆信息窗口的消息
#define WM_SETMARKERVISABLE				WM_USER+4 // 设置地图标注可见的消息

/////////////////////////////////////////////////////////////////////////////
// CCarManageApp:
// See CarManage.cpp for the implementation of this class
//
class CImpIDispatch;

class CCarManageApp : public CWinApp
{
public:
	CCarManageApp();

public:
	CImpIDispatch*		m_pCustDisp;
	std::map<int, int>	topDlgLst;
	int					unit;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCarManageApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CCarManageApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG

private:

	DECLARE_MESSAGE_MAP()
};

extern CCarManageApp theApp;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CARMANAGE_H__0EE0D8C0_D500_4718_93CA_7B7160FE3146__INCLUDED_)
