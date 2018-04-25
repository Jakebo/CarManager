// UserOpt.h: interface for the CUserOpt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USEROPT_H__F0ED8C4A_48EB_451D_84E0_D2792626133A__INCLUDED_)
#define AFX_USEROPT_H__F0ED8C4A_48EB_451D_84E0_D2792626133A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <string>

class CUserOpt  
{
public:
	CUserOpt();

	// 获取所有的用户名及其ID
	void GetUser(std::map<int, CString> &userLst);
	const CString GetUserPhoneById(const int &id);

	virtual ~CUserOpt();

};

#endif // !defined(AFX_USEROPT_H__F0ED8C4A_48EB_451D_84E0_D2792626133A__INCLUDED_)
