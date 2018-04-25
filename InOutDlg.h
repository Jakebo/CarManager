#if !defined(AFX_INOUTDLG_H__FEB08BB2_1112_43C9_9304_46B5CEE49110__INCLUDED_)
#define AFX_INOUTDLG_H__FEB08BB2_1112_43C9_9304_46B5CEE49110__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InOutDlg.h : header file
//
#include "DriverOpt.h"
#include "CarOpt.h"
#include "InOutOpt.h"
#include "XComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CInOutDlg dialog

class CInOutDlg : public CDialog
{
// Construction
public:
	CInOutDlg(CWnd* pParent = NULL);   // standard constructor
	CInOutDlg(BOOL bInOut, CWnd *pParent = NULL);
	CInOutDlg(BOOL bInOut, std::vector<std::string> &content, CWnd *pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CInOutDlg)
	enum { IDD = IDD_INOUTDLG };
	CXComboBox	m_usedept;
	CDateTimeCtrl	m_backctrl;
	CDateTimeCtrl	m_usectrl;
	CXComboBox	m_whobackcombox;
	CXComboBox	m_whopastcombox;
	CXComboBox	m_whooutcombox;
	CXComboBox	m_usercombox;
	CXComboBox	m_numbercombox;
	CXComboBox	m_drivercombox;
	CString	m_comment;
	CString	m_driverphone;
	CTime	m_usetime;
	CTime	m_backtime;
	CString	m_phone;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInOutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Attiure
private:
	CDriverOpt	m_driverOpt; // 驾驶员操作对象
	CCarOpt		m_carOpt; // 车辆操作对象
	CInOutOpt	m_ioOpt; // 派/收车管理对象
	BOOL		m_bInOut; // TRUE : 派车， FALSE : 收车
	std::vector<std::string> content; // 保存窗口内容
	

// Implementation
protected:
	void InitInterface();
	void InitCombox();
	BOOL DetectCondition(); // 检测用户输入的是否满足条件
	BOOL NewOutCar();
	BOOL BackCar();
	void GetTextContent();

public:
	const std::vector<std::string> &GetContent();
	void SetContent(std::vector<std::string> &cot);
	// Generated message map functions
	//{{AFX_MSG(CInOutDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSave();
	afx_msg void OnSelchangeDriver();
	afx_msg void OnSelchangeNumber();
	afx_msg void OnSelchangeUser();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INOUTDLG_H__FEB08BB2_1112_43C9_9304_46B5CEE49110__INCLUDED_)
