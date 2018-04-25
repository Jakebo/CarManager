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

// QueryDriver() : ��ѯ��ʻԱ�� ID �� ����
// res : ���淵�ؽ��
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

// QueryPhoById() : ���ݼ�ʻԱ ID ��ѯ��ʻԱ�绰
// id : ��Ҫ��ѯ�� id
// phone : ���ص绰
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

