// DeptOpt.cpp: implementation of the CDeptOpt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CarManage.h"
#include "DeptOpt.h"
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

CDeptOpt::CDeptOpt()
{

}

CDeptOpt::~CDeptOpt()
{

}

// GetDept() : ��ѯ���еĲ���
// res : ���沢���ز�ѯ���
void CDeptOpt::GetDept(std::map<int, CString> &res)
{
	_RecordsetPtr pRst;
	CString strSql;

	strSql = "Select id,name from depts";
	
	try {
		pRst = conn.GetRecordSet(strSql);
		while (!pRst->adoEOF) {
			res[pRst->GetCollect("id").intVal] = 
				pRst->GetCollect("name").bstrVal;

			pRst->MoveNext();
		}
	}
	catch (...) {
	}
}
