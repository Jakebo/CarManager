//{{AFX_INCLUDES()
#include "webbrowser2.h"
#include "dtpicker.h"
//}}AFX_INCLUDES
#if !defined(AFX_PLAYBACKDLG_H__EA1EB437_BEC2_4C50_BB53_9BCB26DAC0F6__INCLUDED_)
#define AFX_PLAYBACKDLG_H__EA1EB437_BEC2_4C50_BB53_9BCB26DAC0F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlayBackDlg.h : header file
//

#include "WebPage.h"
#include "TargetOpt.h"

/////////////////////////////////////////////////////////////////////////////
// CPlayBackDlg dialog

class CPlayBackDlg : public CDialog
{
// Construction
public:
	CPlayBackDlg(CWnd* pParent = NULL);   // standard constructor
	CPlayBackDlg(const int targetID,
		const CString name,
		CWnd* pParent = NULL);

	void Resize();
	void SelectTime (); // 设置选择时间控件的可见性
	void ShowPBMap (); // 设置回放地图的可见性
	void InitTime(); // 初始化时间控件的初始时间
	void InitPlayback(); // 初始化轨迹回放所需的各个参数
	void WriteDataToWeb();
	BOOL GetCollect(_RecordsetPtr &pRec, LPCTSTR Name, float &res);
	void DownloadLocus();
	void MoveWebWindow();

// Dialog Data
	//{{AFX_DATA(CPlayBackDlg)
	enum { IDD = IDD_PLAYBACKDLG };
	CButton	m_selectTime;
	CStatic	m_msgLab;
	CStatic	m_eSecLab;
	CStatic	m_sSecLab;
	CComboBox	m_esec;
	CComboBox	m_ssec;
	CButton	m_startPB;
	CProgressCtrl	m_dlPro;
	CButton	m_replayBtn;
	CStatic	m_endLab;
	CStatic	m_eMinLab;
	CStatic	m_eHourLab;
	CStatic	m_sMinLab;
	CStatic	m_sHourLab;
	CStatic	m_startLab;
	CButton	m_cancelbtn;
	CButton	m_okbtn;
	CComboBox	m_emin;
	CComboBox	m_ehour;
	CComboBox	m_smin;
	CComboBox	m_shour;
	CWebBrowser2	m_playbackWeb;
	CDTPicker	m_sPicker;
	CDTPicker	m_ePicker;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlayBackDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	POINT old; // Store the dialog's width & height
	CTargetOpt targetOpt; // Target 操作对象
	CWebPage m_webPage;
	BOOL hasMapFile;
	int targetID; // 被轨迹回放的车辆的ID
	CString targetName; // 名字
	static CString startTime; // 选择的起始时间
	static CString endTime; // 选择的结束时间
	_RecordsetPtr pRst; // 车辆轨迹记录集
	CStringArray args; // 写到Web页面的参数列表
	CString longitude, latitude; // 车辆的经纬度
	CString time; // 时间
	CString speed; // 速度
	int interval; // 地图更新的时间间隔
	BOOL showMap; // 是否显示地图，因为ActiveX控件的不一般性，所以我用这种笨方法@_@

	// Generated message map functions
	//{{AFX_MSG(CPlayBackDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnRePlay();
	afx_msg void OnStartPB();
	virtual void OnCancel();
	afx_msg void OnSelectTime();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYBACKDLG_H__EA1EB437_BEC2_4C50_BB53_9BCB26DAC0F6__INCLUDED_)
