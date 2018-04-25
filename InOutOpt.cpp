// InOutOpt.cpp: implementation of the CInOutOpt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CarManage.h"
#include "InOutOpt.h"
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

CInOutOpt::CInOutOpt()
{

}

CInOutOpt::~CInOutOpt()
{

}

// QueryRecord() : ��ѯ���е��ɳ���¼
// pRst : �����ѯ�����¼��������
void CInOutOpt::QueryRecord(_RecordsetPtr &pRst)
{
	CString strSql;

	strSql = "select outcar.coding,number,driver,driverphone,[user],\
				userphone,deptname=(case when dept is not null then \
				(select name from depts where depts.id=outcar.dept) else '' end),\
				usetime,backtime,startplace,\
				viaplace,destination,usereason,whoout,whopast,entertime,\
				backstatus=(case when isback=1 then '����' else 'δ��' end),\
				whoback,comment \
				from outcar";

	try {
		pRst = conn.GetRecordSet(strSql);
	}
	catch (_com_error e){
	}
}

// QueryRecord() : ������ѯ�ɳ���¼
// pRst : �����ѯ�����¼��������
// searchText : �����ַ���
// select : ��ʾ����ѡ��Ҫ��ѯ�ĳ�������0 ��ʾ��ѯ���г�����
//			1 ��ʾ��ѯ���ճ�����2 ��ʾ��ѯδ�ճ���
void CInOutOpt::QueryRecord(_RecordsetPtr &pRst,
							const std::string &searchText,
							const UCHAR &select)
{
	std::string strSql;

	strSql = "select outcar.coding,number,driver,driverphone,[user],\
				userphone,deptname=(case when dept is not null then \
				(select name from depts where depts.id=outcar.dept) else '' end),\
				usetime,backtime,startplace,\
				viaplace,destination,usereason,whoout,whopast,entertime,\
				backstatus=(case when isback=1 then '����' else 'δ��' end),\
				whoback,comment \
				from outcar \
				where " + searchText;

	switch (select) {
	case 1:
		strSql += " and isback=1"; // ��ѯ���ճ���
		break;
	case 2:
		strSql += " and (isback=0 or isback is null)"; // ��ѯδ�ճ���
		break;
	}

	try {
		pRst = conn.GetRecordSet(strSql.data());
	} 
	catch (_com_error e) {
	}
}

// QueryUnBackRecord() : ��ѯ����/δ�ճ�����¼
// pRst : �����ѯ�����¼��
void CInOutOpt::QueryRecord(_RecordsetPtr &pRst, BOOL isBack)
{
	std::string strSql;

	strSql = "select outcar.coding,number,driver,driverphone,[user],\
				userphone,deptname=(case when dept is not null then \
				(select name from depts where depts.id=outcar.dept) else '' end),\
				usetime,backtime,startplace,\
				viaplace,destination,usereason,whoout,whopast,entertime,\
				backstatus=(case when isback=1 then '����' else 'δ��' end),\
				whoback,comment \
				from outcar\
				where ";

	if (isBack) {
		strSql += "isback=1";
	} else {
		strSql += "isback=0 or isback is null";
	}

	try {
		pRst = conn.GetRecordSet(strSql.data());
	}
	catch (_com_error e) {
	}
}

// QueryRecordNumByDate() : ��ѯĳ����ɳ���
// date : ĳ�������
// ����ֵ : ���ز�ѯ���������������ɳ�������0��
//			�����ѯ�����쳣���� -1
int CInOutOpt::QueryRecordNumByDate(const std::string &date)
{
	_RecordsetPtr pRst;
	std::string strSql;
	int num;

	strSql = "select count(id) num from outcar where entertime='" + date + "'";

	try {
		pRst = conn.GetRecordSet(strSql.data());
		if (!pRst->adoEOF) {
			num = pRst->GetCollect("num").intVal;
		} else {
			num = 0;
		}
	}
	catch (_com_error e) {
		num = -1;
	}

	return num;
}

// NewOutCarRecord() : ����һ���ɳ���¼
// content : ��¼������
void CInOutOpt::NewOutCarRecord(std::vector<std::string> &content)
{
	_ConnectionPtr pConn = conn.GetConnPtr();
	_RecordsetPtr pRst;
	std::string strSql;
	_variant_t var;
	std::vector<std::string>::iterator begin, end;
	
	begin = content.begin();
	end   = content.end();
	--end;

	strSql = "SELECT * FROM outcar where id in( \
				select id from sysobjects \
				where xtype='u' and [name]='�豸����')";
	
	try {
		// ��ȡ��������ֶ�
		pRst = pConn->Execute(strSql.data(),NULL,adCmdText);

		UINT cols = pRst->GetFields()->Count; // �ֶ���
		strSql = "insert into outcar([";
		// ���������ֶβ���ϳ� SQL ���
		for (int i = 1; i < cols - 1; ++i) {
			var = pRst->GetFields()->Item[long(i)]->GetName();
			strSql += (_bstr_t)var.bstrVal + "],[";
		}

		var = pRst->GetFields()->Item[long(i)]->GetName();
		strSql += (_bstr_t)var.bstrVal + "]) values('";

		while (begin != end) {
			strSql += *begin + "','";
			++begin;
		}
		
		if (begin != content.end()) {
			strSql += *begin + "')";
		}

		conn.ExecuteSQL(strSql.data());
	}
	catch (...) {
	}
}


// InComeCar() : �ճ�
// content:�ճ����µ�����
void CInOutOpt::InComeCar(std::vector<std::string> &content)
{
	_ConnectionPtr pConn = conn.GetConnPtr();
	_RecordsetPtr pRst;
	std::string strSql;
	std::string tmp;
	_variant_t var;
	std::vector<std::string>::iterator begin;
	
	begin = content.begin() + 2;

	strSql = "SELECT * FROM outcar where id in( \
				select id from sysobjects \
				where xtype='u' and [name]='�豸����')";
	
	try {
		// ��ȡ��������ֶ�
		pRst = pConn->Execute(strSql.data(),NULL,adCmdText);

		UINT cols = pRst->GetFields()->Count; // �ֶ���
		strSql = "update outcar set ";
		// ���������ֶβ���ϳ� SQL ���
		for (int i = 3; i < cols - 1; ++i, ++begin) {
			var = pRst->GetFields()->Item[long(i)]->GetName();
			if (strcmp("entertime", (_bstr_t)var.bstrVal) == 0) {
				continue;
			}
			tmp = (_bstr_t)var.bstrVal;
			strSql += "[" + tmp + "]='" + *begin + "',";
		}

		var = pRst->GetFields()->Item[long(i)]->GetName();
		tmp = (_bstr_t)var.bstrVal;
		strSql += "[" + tmp + "]='" + *begin + "'";

	
		strSql += " where coding='" + *(content.begin()) + "'";

		conn.ExecuteSQL(strSql.data());
	}
	catch (...) {
	}
}
 
// DetectLicit() : ���ĳ���ƺŶ�Ӧ�ĳ�����ĳ��ʱ������Ƿ����
// number : ָ����Ҫ���ĳ��ƺ�
// usetime : ��ʼʹ��ʱ��
// backtime : ���Ʒ���ʱ��
// ����ֵ : ����ó����ڸ�ʱ����ڿ��ɣ����� TRUE�����򷵻� FALSE
BOOL CInOutOpt::DetectLicit(const std::string &number,
							const std::string &usetime,
							const std::string &backtime,
							std::string &conflict)
{
	_RecordsetPtr pRst;
	std::string strSql;

	strSql = "select usetime,backtime from outcar where number='" + number + "' and \
				(('" + usetime + "'>=usetime and '" + usetime + 
				"'<=backtime) or ('" + backtime + "'>=usetime and '" + backtime + 
				"'<=backtime) or ('" + usetime + "'<=usetime and '" + 
				backtime + "'>=backtime))";

	try {
		pRst = conn.GetRecordSet(strSql.data());
		if (pRst->adoEOF) {
			return TRUE; // �����ѯ���Ϊ�գ���ʾ����
		} else {
			std::string tmU;
			std::string tmB;

			while (!pRst->adoEOF) {
				_variant_t date = pRst->GetCollect("usetime");
				tmU		 =  (COleDateTime(date).Format("%Y-%m-%d")).GetBuffer(0);
				date	 =  pRst->GetCollect("backtime");
				tmB		 =  (COleDateTime(date).Format("%Y-%m-%d")).GetBuffer(0);
				conflict += (tmU + "����" + tmB + ",\n");

				pRst->MoveNext();
			}

			return FALSE; // �����ʾ������
		}
	}
	catch (_com_error e) {
	}

	return TRUE;
}