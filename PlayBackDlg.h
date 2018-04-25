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
	void SelectTime (); // ����ѡ��ʱ��ؼ��Ŀɼ���
	void ShowPBMap (); // ���ûطŵ�ͼ�Ŀɼ���
	void InitTime(); // ��ʼ��ʱ��ؼ��ĳ�ʼʱ��
	void InitPlayback(); // ��ʼ���켣�ط�����ĸ�������
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
	CTargetOpt targetOpt; // Target ��������
	CWebPage m_webPage;
	BOOL hasMapFile;
	int targetID; // ���켣�طŵĳ�����ID
	CString targetName; // ����
	static CString startTime; // ѡ�����ʼʱ��
	static CString endTime; // ѡ��Ľ���ʱ��
	_RecordsetPtr pRst; // �����켣��¼��
	CStringArray args; // д��Webҳ��Ĳ����б�
	CString longitude, latitude; // �����ľ�γ��
	CString time; // ʱ��
	CString speed; // �ٶ�
	int interval; // ��ͼ���µ�ʱ����
	BOOL showMap; // �Ƿ���ʾ��ͼ����ΪActiveX�ؼ��Ĳ�һ���ԣ������������ֱ�����@_@

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
