#if !defined(AFX_EDITCARDATADLG_H__2FAE635E_BB81_4C4E_9BA7_6AB0D6BFA156__INCLUDED_)
#define AFX_EDITCARDATADLG_H__2FAE635E_BB81_4C4E_9BA7_6AB0D6BFA156__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditCarDataDlg.h : header file
//
#include "XComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CEditCarDataDlg dialog
class CCarOpt;
class CDriverOpt;

class CEditCarDataDlg : public CDialog
{
// Construction
public:
	CEditCarDataDlg(CWnd* pParent = NULL);   // standard constructor
	CEditCarDataDlg(BOOL addNew, CWnd *pParent = NULL);
	CEditCarDataDlg(BOOL addNew,
					const CStringArray &content,
					BOOL editAble,
					CWnd* pParent = NULL);
	virtual ~CEditCarDataDlg();

	void InitInterface(); // ��ʼ�û�����
	void InitCombox(); // ��ʼ�� COMBOX
	void GetContent(CStringArray &content); // ��ȡ�û��������������
	void SetContent(const CStringArray &content); // �����޸�����ʱ���ô�������

// Dialog Data
	//{{AFX_DATA(CEditCarDataDlg)
	enum { IDD = IDD_EDITCARDATADLG };
	CXComboBox	m_targetbox;
	CXComboBox	m_cartypecombox;
	CXComboBox	m_carbrandcombox;
	CXComboBox	m_belongDept;
	CXComboBox	m_carstatus;
	CXComboBox	m_driverbox;
	//}}AFX_DATA

	BOOL			addNew; // ������༭�������ϵı�־��TRUE:����,FALSE:�༭
	BOOL			editAble; // ���ڵĿؼ��Ƿ�ɱ༭
	CCarOpt			*carOpt; // �����������ϵĶ���
	CDriverOpt		*driverOpt; // ������ʻԱ����
	CStringArray	content;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditCarDataDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditCarDataDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITCARDATADLG_H__2FAE635E_BB81_4C4E_9BA7_6AB0D6BFA156__INCLUDED_)
