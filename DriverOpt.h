// DriverOpt.h: interface for the CDriverOpt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRIVEROPT_H__1955693C_4539_446E_B19F_5FE4396D30DC__INCLUDED_)
#define AFX_DRIVEROPT_H__1955693C_4539_446E_B19F_5FE4396D30DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <string>

class CDriverOpt  
{
public:
	CDriverOpt();
	virtual ~CDriverOpt();

	// 查询驾驶员的 ID 和 名字
	void QueryDriver(std::map<int, CString> &res);
	// 根据驾驶员的 ID 查询驾驶员电话
	void QueryPhoById(const int &id, std::string &phone);
};

#endif // !defined(AFX_DRIVEROPT_H__1955693C_4539_446E_B19F_5FE4396D30DC__INCLUDED_)
