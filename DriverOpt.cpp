// DriverOpt.cpp: implementation of the CDriverOpt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CarManage.h"
#include "DriverOpt.h"
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

CDriverOpt::CDriverOpt()
{

}

CDriverOpt::~CDriverOpt()
{

}

// QueryDriver() : 查询驾驶员的 ID 和 名字
// res : 保存返回结果
void CDriverOpt::QueryDriver(std::map<int, CString> &res)
{
	_RecordsetPtr pRst;
	CString strSql;
	CString name;

	strSql = "select id,name from driver";

	try {
		pRst = conn.GetRecordSet(strSql);

		while (!pRst->adoEOF) {
			name = pRst->GetCollect("name").bstrVal;
			res[pRst->GetCollect("id").intVal] = name;

			pRst->MoveNext();
		}
	}
	catch (...) {
	}
}

// QueryPhoById() : 根据驾驶员 ID 查询驾驶员电话
// id : 将要查询的 id
// phone : 返回电话
void CDriverOpt::QueryPhoById(const int &id,
							  std::string &phone)
{
	_RecordsetPtr pRst;
	std::string strSql;
	std::string strId;
	char number[10] = { 0 };

	itoa(id, number, 10);
	strId = number; 
	strSql = "select phone from driver where id=" + strId;

	try {
		pRst = conn.GetRecordSet(strSql.data());
		if (!pRst->adoEOF) {
			if (VT_NULL != pRst->GetCollect("phone").vt) {
				phone = (_bstr_t)pRst->GetCollect("phone").bstrVal;
			}
		}
	}
	catch (_com_error e) {
	}
}

