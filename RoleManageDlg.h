#if !defined(AFX_ROLEMANAGEDLG_H__81982EC3_3928_4D4F_A4C9_35207DAB1D4D__INCLUDED_)
#define AFX_ROLEMANAGEDLG_H__81982EC3_3928_4D4F_A4C9_35207DAB1D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RoleManageDlg.h : header file
//

#include <vector>

#include "RoleOpt.h"
#include "XCListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CRoleManageDlg dialog

class CRoleManageDlg : public CDialog
{
// Construction
// Ĭ�ϵĹ��캯���Ѿ���ɾ��
public:
	CRoleManageDlg(CString &purviewCoding, CWnd *pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CRoleManageDlg)
	enum { IDD = IDD_ROLEDLG };
	CListBox	m_userLst;
	CTreeCtrl	m_perThree;
	CXCListCtrl	m_roleLst;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRoleManageDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetPerTreeData(); // ����Ȩ����������
protected:
	void SetUserLstData(std::map<int, CString> &userMap);
	void SetRoleLstData(std::map<int, CString> &roleMap);
	void SetRoleLstCheck(std::map<int, CString> &userRoleMap);
	void SetRoleLstNoCheck();
	// ��ȡ���ڵ�����
	void GetWndChild(BOOL haveParent, HTREEITEM &parentItem, CWnd *pWnd); 
	void SetPerTreeCheck(HTREEITEM &hTree, CString &purviewCoding);
	void SetPerTreeNoCheck();
	// ��ȡȨ������ѡ���������������ӦȨ�ޱ���
	void GetPerTreeCheck(CString &purviewCoding); 
	void RecursionForGetCheck(HTREEITEM &hTree,
								CString &purviewCoding);
	void ModuleSetup(CString &purviewCoding); // ģ�����ú���

	// Save the master-role relation
	void SaveMRRelation();
	// Save the role-action relation
	void SaveRolePurviewCoding(CString &purviewCoding);

	// Get diffirent user-role at the role list
	void GetDiffRoleAtRoleLst(CListCtrl &roleLst,
		std::map<int, CString> &addMR,
		std::map<int, CString> &delMR,
		std::map<int, CString> &URMap);

	// Recursion tree
	void RecursionTree(HTREEITEM &hRoot);
	// ��ĳһ���checkboxѡ��״̬�ı�ʱ�Ĵ�����
	void CheckChange(HTREEITEM &selItem);

	void Resize();

	// Generated message map functions
	//{{AFX_MSG(CRoleManageDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeUserLst();
	afx_msg void OnItemchangedRoleLst(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSavePermission();
	afx_msg void OnNewRole();
	afx_msg void OnDelRole();
	afx_msg void OnClickTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDelUser();
	afx_msg void OnNewUser();
	afx_msg void OnModiInfo();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG

private:
	CString &purviewCoding; // ����Ȩ�ޱ���
	CRoleOpt ro;
	POINT old; // To store the older rect
	std::map<int, CString> roleMap;
	std::map<int, CString> userMap;
	std::map<int, CString> userRoleMap;

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROLEMANAGEDLG_H__81982EC3_3928_4D4F_A4C9_35207DAB1D4D__INCLUDED_)
