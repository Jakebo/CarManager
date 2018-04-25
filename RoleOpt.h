// RoleOpt.h: interface for the CRoleOpt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROLEOPT_H__1FBA5E4C_9E44_406C_A093_67D73DC229C9__INCLUDED_)
#define AFX_ROLEOPT_H__1FBA5E4C_9E44_406C_A093_67D73DC229C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <vector>

class CRoleOpt  
{
public:
	CRoleOpt();

	void QueryUserPurviewByName(const int &id, CString &purviewCoding);
	void QueryUserByName( std::map<int, CString> &userMap, const LPCTSTR name = NULL);
	void QueryRoleByUserID(const int &userID, std::map<int, CString> &userRoleMap);
	void QueryPermissByRID(const int &roleID, CString &purviewCoding);
	void QueryRole(std::map<int, CString> &roleLst);
	void GetUserPermissByName(const CString &name, std::map<int, BOOL> &m_permissMap);
	CStringArray &GetUserRoleByName(const CString &name);

	// Add master-role records into MasterRole
	void AddRoleIntoMasterRole(const int &mId,
		const int &cmId,
		const CString &datetime,
		std::map<int, CString> &addMR);
	// Delete master-role records from MasterRole
	void DelRoleFromMasterRole(const int &mId, std::map<int, CString> &delMR);

	// 保存角色ID对应的权限编码
	void SaveRPC(const int &roleID,
		const int &cmId,
		const CString &time,
		const CString &purviewCoding);

	// Add role
	void AddRole(const CString &rName, const int &cmID, const CString &time);
	// Delete role
	BOOL DelRole(const int &rID);
	// 删除用户
	BOOL DelUser(const int &uId);

	virtual ~CRoleOpt();

private:
//	std::map<int, BOOL> m_permissMap;

};

#endif // !defined(AFX_ROLEOPT_H__1FBA5E4C_9E44_406C_A093_67D73DC229C9__INCLUDED_)
