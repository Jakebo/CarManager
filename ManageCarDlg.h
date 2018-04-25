#if !defined(AFX_MANAGECARDLG_H__530EB591_3400_47F0_B77A_96BB45BC746E__INCLUDED_)
#define AFX_MANAGECARDLG_H__530EB591_3400_47F0_B77A_96BB45BC746E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ManageCarDlg.h : header file
//
#include "SizingDialog.h"
#include "XCListCtrl.h"

#include <vector>

/////////////////////////////////////////////////////////////////////////////
// CManageCarDlg dialog
class CInOutOpt;

class CManageCarDlg : public CSizingDialog
{
// Construction
public:
	// Ĭ�Ϲ��캯���Ѿ���ɾ��
	CManageCarDlg(CString &purviewCoding, CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CManageCarDlg)
	enum { IDD = IDD_MANAGECAR };
	CDateTimeCtrl	m_endctrl;
	CDateTimeCtrl	m_startctrl;
	CXCListCtrl	m_list;
	CString	m_dept;
	CTime	m_starttime;
	CTime	m_endtime;
	CString	m_dest;
	CString	m_user;
	//}}AFX_DATA
	CString &purviewCoding;
	// RADIO ��ѡ���1 Ϊ�ɳ���¼��2 Ϊ�ճ���¼��3 Ϊδ�ճ���
	UCHAR m_radSelect;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CManageCarDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void AddCtrl();
	void ModuleSetup(CString &purviewCoding); // Ȩ��ģ������
	void InitList(); // ��ʼ���б�
	void FillList(_RecordsetPtr &res);
	// ��һ����¼��ӵ��б�β��
	void AddToLstTail(std::vector<std::string> &content);
	// ����ĳ�м�¼������
	void UpdateLstRow(const int &nIndex, std::vector<std::string> &content);
	void GetIndexContent(const UINT &nIndex, std::vector<std::string> &content);

private:
	CInOutOpt *m_ioOpt;
	// Generated message map functions
	//{{AFX_MSG(CManageCarDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSearch();
	afx_msg void OnClear();
	afx_msg void OnInRecord();
	afx_msg void OnUnInRecord();
	afx_msg void OnOutRecord();
	afx_msg void OnOutCar();
	afx_msg void OnBackCar();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANAGECARDLG_H__530EB591_3400_47F0_B77A_96BB45BC746E__INCLUDED_)
