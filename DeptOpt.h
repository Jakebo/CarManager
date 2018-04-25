// DeptOpt.h: interface for the CDeptOpt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEPTOPT_H__5B7F5E24_0E18_4AAD_ADE1_4CD9E8E077BA__INCLUDED_)
#define AFX_DEPTOPT_H__5B7F5E24_0E18_4AAD_ADE1_4CD9E8E077BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDeptOpt  
{
public:
	CDeptOpt();
	virtual ~CDeptOpt();

	// 获取所有的部门列表
	void GetDept(std::map<int, CString> &res);
};

#endif // !defined(AFX_DEPTOPT_H__5B7F5E24_0E18_4AAD_ADE1_4CD9E8E077BA__INCLUDED_)
