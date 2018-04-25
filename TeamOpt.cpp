// TeamOpt.cpp: implementation of the CTeamOpt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CarManage.h"
#include "TeamOpt.h"
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

CTeamOpt::CTeamOpt()
{

}

CTeamOpt::~CTeamOpt()
{

}

// GetTeam() : ��ȡ���еĳ���
// teamLst : �� ID-NAME ��ӳ����ʽ�����沢���س����б�
void CTeamOpt::GetTeam(std::map<int, CString> &teamLst)
{
	_RecordsetPtr pRst;
	CString strSql;

	strSql = "select id,name from team";

	try {
		pRst = conn.GetRecordSet(strSql);
		while (!pRst->adoEOF) {
			teamLst[pRst->GetCollect("id").intVal] =
				pRst->GetCollect("name").bstrVal;

			pRst->MoveNext();
		}
	}
	catch (...) {
	}
}