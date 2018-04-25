// RoleOpt.cpp: implementation of the CRoleOpt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CarManage.h"
#include "RoleOpt.h"

#include "ADOConn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern ADOConn conn;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRoleOpt::CRoleOpt()
{
	
}

CRoleOpt::~CRoleOpt()
{
}

void CRoleOpt::QueryUserPurviewByName(const int &id,
									  CString &purviewCoding)
{
	CString strSql;
	CString strId;

	strId.Format("%d", id);
	strSql = "select purviewcoding \
				from role_action \
				where rid in ( \
					select rid from master_role where mid=" + strId + ")";

	try {
		_RecordsetPtr &pRst = conn.GetRecordSet(strSql);
		purviewCoding = pRst->GetCollect("purviewcoding").bstrVal;
	}
	catch (CADOException e) {
		TRACE(e.m_strMessage);
	}
}

// QueryUserByName() : 查询用户
// name : 提供的用户名，如果用户名为空查询所有用户，如果不为空，查询
//			指定用户，该方法可用来查询指定用户的ID，默认值为空
// userMap : 保存查询结果并返回
void CRoleOpt::QueryUserByName(std::map<int, CString> &userMap,
							   const LPCTSTR name /*=NULL*/)
{
	CString strSql;

	if (name == NULL) {
		strSql = "select id,name from master";
	} else {
		strSql = "select id,name from master where name=\"\
				+ name + \"";
	}

	try {
		_RecordsetPtr &pRst = conn.GetRecordSet(strSql);

		while (!pRst->adoEOF) {
			userMap[(pRst->GetCollect("id")).lVal] =
				(pRst->GetCollect("name")).bstrVal;
			pRst->MoveNext();
		}
	}
	catch (CADOException e) {
		TRACE(e.m_strMessage);
	}
}

void CRoleOpt::QueryRoleByUserID(const int &userID,
								 std::map<int, CString> &userRoleMap)
{
	CString strSql;
	CString id;

	id.Format("%d", userID);
	strSql = "select id,name from role \
				where id in( \
				  select distinct rid from master_role where mid=" + id + ")";

	try {
		_RecordsetPtr &pRst = conn.GetRecordSet(strSql);

		while (!(pRst->adoEOF)) {
			userRoleMap[(pRst->GetCollect("id")).lVal] = 
				(pRst->GetCollect("name")).bstrVal;
			pRst->MoveNext();
		}
	}
	catch (CADOException e) {
		TRACE(e.m_strMessage);
	}
}

// QueryPermissByRID() : 通过角色的ID来查询对应的权限编码
// roleID : 提供的角色ID
// purviewCoding : 保存并返回权限编码
void CRoleOpt::QueryPermissByRID(const int &roleID,
								 CString &purviewCoding)
{
	CString strSql;
	CString id;

	id.Format("%d", roleID);
	strSql = "select purviewCoding from role_action \
				where rid=" + id;

	try {
		_RecordsetPtr &pRst = conn.GetRecordSet(strSql);

		if (!pRst->adoEOF) {
			purviewCoding = pRst->GetCollect("purviewCoding").bstrVal;
		} else {
			purviewCoding = _T("");
		}
	}
	catch (CADOException e) {
		TRACE(e.m_strMessage);
	}
}

void CRoleOpt::QueryRole(std::map<int, CString> &roleLst)
{
	CString strSql;

	strSql = "select * from role";

	try {
		_RecordsetPtr &pRst = conn.GetRecordSet(strSql);

		while (!(pRst->adoEOF)) {
			roleLst[(pRst->GetCollect("id")).lVal] =
			(pRst->GetCollect("name")).bstrVal;
			pRst->MoveNext();
		}
	}
	catch (CADOException e) {
		TRACE(e.m_strMessage);
	}
}

// GetUserPermissByName() : Get user permission by user name
// and save permission in the second argument
void CRoleOpt::GetUserPermissByName(const CString &name,
									std::map<int, BOOL> &m_permissMap)
{
	CStringArray &roleLst = GetUserRoleByName(name);
	int len = roleLst.GetSize();
	CString strSql;

	try {
		for (int i = 0; i < len; ++i) {
			strSql = "select AID from RoleAction \
						where RID=" + roleLst.GetAt(i);
			_RecordsetPtr &pRst = conn.GetRecordSet(strSql);

			while (!(pRst->adoEOF)) {
				m_permissMap[(pRst->GetCollect("AID")).lVal] = TRUE;
				pRst->MoveNext();
			}
		}
	}
	catch (CADOException e) {
		TRACE(e.m_strMessage);
	}

	 delete &roleLst;
}

// GetUserRoleByName() : Get user roles by name
CStringArray &CRoleOpt::GetUserRoleByName(const CString &name)
{
	CStringArray *roleLst = new CStringArray();

	CString strSql = "select RID from MasterRole \
						  where MID=(select MID from Master \
						    where MName='" + name + "')";

	try {
		_RecordsetPtr &pRst = conn.GetRecordSet(strSql);
		while (!(pRst->adoEOF)) {
			roleLst->Add((_bstr_t)pRst->GetCollect("RID"));
			pRst->MoveNext();
		}
	}
	catch (CADOException e) {
		TRACE(e.m_strMessage);
	}

	return *roleLst;
}

// AddRoleIntoMasterRole() : Add records into the MasterRole
//				according the addMR argument
// mId : The master's id of records
// cmId : The master's id of Create records
// datetime : The date of creating
// addMR : The adding of rold's id
void CRoleOpt::AddRoleIntoMasterRole(const int &mId,
									 const int &cmId,
									 const CString &datetime,
									 std::map<int, CString> &addMR)
{
	CString strSql;
	CString masterID;
	CString cMasterID;
	CString roleID;
	std::map<int, CString>::iterator it = addMR.begin();

	masterID.Format("%d", mId);
	cMasterID.Format("%d", cmId);

	try {
		while (it != addMR.end()) {
			roleID.Format("%d", it->first);
			strSql = "insert into master_role(mid,createby,createtime,rid) \
					values(" + masterID + "," + cMasterID + 
					",'" + datetime + "'," + roleID + ")";

			conn.ExecuteSQL(strSql);
			++it;
		}
	}
	catch (CADOException e) {
		TRACE(e.m_strMessage);
	}
}

// DelRoleFromMasterRole() : Delete records from MasterRole
//				according the delMR argument
// mId : The master's id of deleting
// delMR : The role's id of deleting
void CRoleOpt::DelRoleFromMasterRole(const int &mId, std::map<int, CString> &delMR)
{
	CString masterID;
	CString roldID;
	CString strSql;
	std::map<int, CString>::iterator it = delMR.begin();
	
	masterID.Format("%d", mId);

	try {
		while (it != delMR.end()) {
			strSql = "delete from master_role \
						where mid=" + masterID + " and rid=";
			roldID.Format("%d", it->first);
			strSql += roldID;

			conn.ExecuteSQL(strSql);
			++it;
		}
	}
	catch (CADOException e) {
		TRACE(e.m_strMessage);
	}		
}

// SaveRPC() : 保存角色ID对应的权限编码
// roleID : 角色ID
// cmId : 创建或修改该角色的权限编码记录的管理员ID
// time : 创建或修改该角色的权限编码记录的时间
// purviewCoding : 权限编码
void CRoleOpt::SaveRPC(const int &roleID,
					   const int &cmId,
					   const CString &time,
					   const CString &purviewCoding)
{
	CString strSql;
	CString rid;
	CString mid;

	rid.Format("%d", roleID);
	mid.Format("%d", cmId);

	// 查询该角色对应记录的ID
	strSql = "select id from role_action \
				where rid=" + rid;
	try {
		_RecordsetPtr &pRst = conn.GetRecordSet(strSql);
		// 如果存在该角色对应的记录，则只需对该记录进行更新
		if (!pRst->adoEOF) {
			strSql = "update role_action set purviewCoding='" + purviewCoding +
						"',mid=" + mid +",createtime='" + time +
						"' where rid=" + rid;
		} else { // 否则为不存在对应的记录，需要插入一条该角色的新记录
			strSql = "insert into role_action(rid,mid,createtime,purviewCoding) \
						values(" + rid + "," + mid + ",'" + 
								time + "','" + purviewCoding + "')";
		}
		conn.ExecuteSQL(strSql); // 执行SQL语句
	}
	catch (CADOException e) {
		TRACE("更新权限出错: " + e.m_strMessage);
	}
}

// AddRole() : Adding a role into Role table
// rName : the role name
// cmID : The name of the creating role
void CRoleOpt::AddRole(const CString &rName, const int &cmID, const CString &time)
{
	CString masterID;
	CString strSql;

	masterID.Format("%d", cmID);

	try {
		strSql = "insert into role(name,mid,createtime) \
					values('" + rName + "'," + masterID + ",'" + time + "')";

		conn.ExecuteSQL(strSql);
	}
	catch (CADOException e) {
		TRACE(e.m_strMessage);
	}
}

// DelRole() : Delete a role according role's id
// rID : The id of deleted
BOOL CRoleOpt::DelRole(const int &rID)
{
	CString roleID;
	CString strSql;

	roleID.Format("%d", rID);

	try {
		strSql = "delete from role \
					where id=" + roleID;

		conn.ExecuteSQL(strSql);
		return TRUE;
	}
	catch (CADOException e) {
		TRACE(e.m_strMessage);
		return FALSE;
	}
}

// DelUser() : 删除指定ID的用户
// uId : 用户的ID
BOOL CRoleOpt::DelUser(const int &uId)
{
	CString userId;
	CString strSql;

	userId.Format("%d", uId);

	strSql = "delete from master \
				where id=" + userId;

	try {
		conn.ExecuteSQL(strSql);
		return TRUE;
	}
	catch (CADOException e) {
		TRACE(e.m_strMessage);
		return FALSE;
	}
}