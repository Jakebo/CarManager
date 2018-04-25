#if !defined(AFX_GPSDLG_H__799D8DCA_2BA4_426C_8C5E_112A76A97327__INCLUDED_)
#define AFX_GPSDLG_H__799D8DCA_2BA4_426C_8C5E_112A76A97327__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GpsDlg.h : header file
//

#include <map>

#include "SizingDialog.h"
#include "TargetOpt.h"


/////////////////////////////////////////////////////////////////////////////
// CGpsDlg dialog
class CCarInfo;

class CGpsDlg : public CSizingDialog
{
// Construction
// 默认构造已被删掉
public:
	CGpsDlg(CString &purviewCoding, CWnd* pParent = NULL);   // standard constructor
	virtual ~CGpsDlg();

public:
	void SetTreeData(std::vector<Target> &targetRecord);
	// 根据标志创建跟踪、回放或信息窗口
	void CreateTraceWindow(char flag);
	void SetTraceWindowPos();
	void SetCarInfoWndPos();
	void CreateTraceMarker(const int id);
	void ModuleSetup(CString &purviewCoding); 
	void AddCtrl();
	void ShowWindow(BOOL bShow, UINT nStatus);
	void SetMarkerVisable(const int &targetId);
	void SetMarkerVisable(const CString &targetCoding);

// Dialog Data
	//{{AFX_DATA(CGpsDlg)
	enum { IDD = IDD_GPSDLG };
	CTreeCtrl	m_tree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGpsDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString &purviewCoding; // 保存权限编码
	CTargetOpt targetOpt;
	std::map<int, CDialog*> m_traceDlg; // 跟踪窗口列表
	std::map<int, CDialog*> m_pBDlg; // 回放窗口列表
	std::vector<Target> targetRecord;
	CCarInfo	*carInfo;

	// Generated message map functions
	//{{AFX_MSG(CGpsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTraceCar();
	afx_msg void OnPaint();
	afx_msg LRESULT DeleteTraceDlg(WPARAM iParam1, LPARAM iParam2);
	afx_msg LRESULT DeletePBDlg(WPARAM iParam1, LPARAM iParam2);
	afx_msg LRESULT DeleteCarInfoDlg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT SetMarkerVisable(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPlayBack();
	afx_msg void OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCarInfo();
	afx_msg void OnSelchangedCarTree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GPSDLG_H__799D8DCA_2BA4_426C_8C5E_112A76A97327__INCLUDED_)
