// UserOpt.cpp: implementation of the CUserOpt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CarManage.h"
#include "UserOpt.h"
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

CUserOpt::CUserOpt()
{

}

CUserOpt::~CUserOpt()
{

}

// GetUser() : 获取所有用户名及其ID
// userLst : 保存并返回结果
void CUserOpt::GetUser(std::map<int, CString> &userLst)
{
	_RecordsetPtr pRst;
	std::string   strSql;

	strSql = "select id,name from master";

	try {
		pRst = conn.GetRecordSet(strSql.data());
		while (!pRst->adoEOF) {
			userLst[pRst->GetCollect("id").intVal] =
					pRst->GetCollect("name").bstrVal;
			pRst->MoveNext();
		}				
	}
	catch (...) {
	}
}

// GetUserPhoneById() : 通过用户 ID 获取用户电话号码
// id : 指定的用户 ID
const CString CUserOpt::GetUserPhoneById(const int &id)
{
	_RecordsetPtr pRst;
	CString strSql;
	CString tmpStr;

	tmpStr.Format("%d", id);

	strSql = "select mobile from master where id=" + tmpStr;

	try {
		pRst = conn.GetRecordSet(strSql);
		if (!pRst->adoEOF) {
			if (VT_NULL != pRst->GetCollect("mobile").vt) {
				tmpStr = pRst->GetCollect("mobile").bstrVal;
			} else {
				tmpStr = "";
			}

			return tmpStr;
		}
	}
	catch (_com_error e) {
	}

	return "";
}