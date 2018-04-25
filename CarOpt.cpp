// CarOpt.cpp: implementation of the CCarOpt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CarManage.h"
#include "CarOpt.h"
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

CCarOpt::CCarOpt()
{

}

CCarOpt::~CCarOpt()
{

}

// QueryAllCarInfo() : ��ѯ���еĳ�����Ϣ
// res : �����ѯ���������
void CCarOpt::QueryAllCarInfo(_RecordsetPtr &res)
{
	CString strSql;

	strSql = "select id,coding,number,carbrand,cartype,belongdept,\
				status=(case when carstatus=0 then '����' else \
				case when carstatus=1 then '����' else \
				case when carstatus=2 then 'ά��' else \
				case when carstatus=3 then 'ͣ��' else \
				case when carstatus=4 then '����' else \
				case when carstatus=5 then '����' end end end end end end),\
				driver,buytime,entertime,terminalnum  \
				from cardata";
	
	try {
		res = conn.GetRecordSet(strSql);
	}
	catch (...) {
	}
}

// SearchByText() : �Դ��ݽ������ı�����ģ����ѯ
// text : ��Ҫ�����������ı�
// pRst : �����������
void CCarOpt::SearchByText(const CString &text, _RecordsetPtr &pRst)
{
	CString strSql;

	// �����ݿ����ģ����ѯ����䣬��ѯ���ֶ��У�
	// number(����)��(����Ʒ��)��(��������)��(��������)��(��ʻԱ)��(�ն˺���)
	strSql = "select id,coding,number,carbrand,cartype,belongdept,\
				status=(case when carstatus=0 then '����' else \
				case when carstatus=1 then '����' else \
				case when carstatus=2 then 'ά��' else \
				case when carstatus=3 then 'ͣ��' else \
				case when carstatus=4 then '����' else \
				case when carstatus=5 then '����' end end end end end end),\
				driver,buytime,entertime,terminalnum  \
				from cardata \
				where number like '%" + text + "%' or carbrand like '%" + text + 
				"%' or cartype like '%" + text + "%' or belongdept like '%" + text + 
				"%' or driver like '%" + text + "%' or terminalnum like '%" + text + "%'";

	try {
		pRst = conn.GetRecordSet(strSql);
	}
	catch (...) {
	}
}

// GetCountByDate() : ���ݵǼ����ڲ�ѯͬһ���¼���������������ɱ���
// date : ��Ҫ���в�ѯ������
// ����ֵ : ����ָ���Ǽ����ڵĵ�ͬһ��ļ�¼�����������ѯ
//			�����쳣���� -1�����û��ͬһ��Ǽǵļ�¼���� 0
int CCarOpt::GetCountByDate(const COleDateTime &date)
{
	CString strSql;
	CString strDate;

	strDate = date.Format("%Y-%m-%d");

	// ��ѯ��¼��������䣬��ѯ��������� res ��
	strSql = "select COUNT(coding) res from cardata \
				where entertime='" + strDate + "'";

	try {
		_RecordsetPtr &pRst = conn.GetRecordSet(strSql);

		if (!pRst->adoEOF) {
			return pRst->GetCollect("res").intVal;
		}
	}
	catch (...) {
	}

	return -1;
}

// GetCountByNumber() : ��ѯ��ĳ���ƺŶ�Ӧ�ĳ�����
//		�����ж��Ƿ����ظ��ĳ��ƺ�
// number : ��Ҫ���в�ѯ�ĳ��ƺ�
// ����ֵ : ������ָ�����ƺŶ�Ӧ�ĳ������������û��
//			��Ӧ�ĳ������� 0�������ѯ�����쳣�򷵻� -1
int CCarOpt::GetCountByNumber(const CString &number) {
	CString strSql;

	strSql = "select count(number) res from cardata \
				where number='" + number + "'";

	try {
		_RecordsetPtr &pRst = conn.GetRecordSet(strSql);
		
		if (!pRst->adoEOF) {
			return pRst->GetCollect("res").intVal;
		}
	}
	catch (...) {
	}

	return -1;
}


// NewCarData() : ����һ����¼
// args : ����������¼������
BOOL CCarOpt::NewCarData(const CStringArray &args)
{
	CString strSql;
	int size;
	int index;

	strSql = "insert into cardata(coding,number,carbrand,cartype,belongdept,carstatus,\
				driver,buytime,entertime,terminalnum) values('";
	
	size = args.GetSize();

	// ѭ����ȡ���ݽ����Ĳ���
	for (index = 0; index < (size - 1); ++index) {
		strSql += (args.GetAt(index) + "','");
	}

	strSql += args.GetAt(index) + "')"; // ������һ������

	try {
		conn.ExecuteSQL(strSql); // ִ�� SQL ���
	}
	catch (...) {
		return FALSE;
	}

	return TRUE;
}

// ModiCarData() : �޸ĳ�������
// args : ��Ҫ���µ�����
BOOL CCarOpt::ModiCarData(const CStringArray &args)
{
	_ConnectionPtr pConn = conn.GetConnPtr();
	_RecordsetPtr pRst;
	CString strSql;

	strSql = "SELECT * FROM cardata where id in( \
				select id from sysobjects \
				where xtype='u' and [name]='�豸����')";
	
	try {
		// ��ȡ��������ֶ�
		pRst = pConn->Execute(_bstr_t(strSql),NULL,adCmdText);

		CString fieldStr;
		_variant_t var;
		UINT cols = pRst->GetFields()->Count; // �ֶ���
		strSql = "update cardata set ";
		// ���������ֶβ���ϳ� SQL ���
		for (int i = 2; i < cols - 1; ++i) {
			var = pRst->GetFields()->Item[long(i)]->GetName();
			fieldStr = var.bstrVal;
			strSql += fieldStr + "='" + args.GetAt(i - 1) + "',";
		}
		var = pRst->GetFields()->Item[long(i)]->GetName();
		fieldStr = var.bstrVal;
		strSql += fieldStr + "='" + args.GetAt(cols - 2) + "' ";

		strSql += "where coding='" + args.GetAt(0) + "'";

		conn.ExecuteSQL(strSql);
	}
	catch (_com_error e) {
		return FALSE;
	}

	return TRUE;
}

// DelCarData() : ɾ�������Ŷ�Ӧ�ĳ�������
// number : ָ���ı���
BOOL CCarOpt::DelCarData(const CString &number)
{
	CString strSql;
	
	strSql = "delete from cardata where number='" + number + "'";

	try {
		conn.ExecuteSQL(strSql);
	}
	catch (_com_error e) {
		return FALSE;
	}

	return TRUE;
}

// GetIdByCoding() : ��ѯ��ָ�������Ӧ�ļ�¼��ID������
// coding : ָ���ı���
// ����ֵ : ����ָ�������Ӧ��ID����������쳣��û�иñ����Ӧ��
//			��¼���򷵻� -1
int CCarOpt::GetIdByCoding(const CString &coding)
{
	CString strSql;

	strSql = "select id from cardata \
				where coding='" + coding + "'";

	try {
		_RecordsetPtr &pRst = conn.GetRecordSet(strSql);

		if (!pRst->adoEOF) {
			return pRst->GetCollect("id").intVal;
		}
	}
	catch (...) {
	}

	return -1;
}

// GetNumber() : ��ѯ������ID�ͳ��ƺ�
// res : �����ѯ���������
void CCarOpt::GetNumber(std::map<int, CString> &res)
{
	_RecordsetPtr pRst;
	CString strSql;

	strSql = "select id,number from cardata";

	try {
		pRst = conn.GetRecordSet(strSql);
		while (!pRst->adoEOF) {
			res[pRst->GetCollect("id").intVal] =
				pRst->GetCollect("number").bstrVal;

			pRst->MoveNext();
		}
	}
	catch (_com_error e) {
	}		
}

// GetDriverById() : ��ѯ��ĳ ID �Ŷ�Ӧ�ĳ�����ʻԱ
// id : �����Ĳ�ѯ ID
// ����ֵ : �����ѯ�ɹ����ز�ѯ���
void CCarOpt::GetDriverById(const int &id, std::string &driver)
{
	_RecordsetPtr pRst;
	std::string strSql;
	std::string strId;
	char number[10] = { 0 };
	
	itoa(id, number, 10);
	strId = number;

	strSql = "select driver from cardata where id=" + strId;
	
	try {
		pRst = conn.GetRecordSet(strSql.data());
		if (!pRst->adoEOF) {
			if (VT_NULL != pRst->GetCollect("driver").vt) {
				driver = (_bstr_t)pRst->GetCollect("driver").bstrVal;
			}
		}
	}
	catch (_com_error e) {
	}
}

// GetCarType() : ��ȡ���������б�
// typeLst : ���沢���س��������б�
void CCarOpt::GetCarType(CStringArray &typeLst)
{
	_RecordsetPtr pRst;
	CString strSql;

	strSql = "select distinct cartype from cardata";

	try {
		pRst = conn.GetRecordSet(strSql);

		while (!pRst->adoEOF) {
			typeLst.Add(pRst->GetCollect("cartype").bstrVal);

			pRst->MoveNext();
		}
	}
	catch (...) {
	}
}

// GetCarBrand() : ��ȡ�����̱��б�
// brandLst : ���沢���س����̱��б�
void CCarOpt::GetCarBrand(CStringArray &brandLst)
{
	_RecordsetPtr pRst;
	CString strSql;

	strSql = "select distinct carbrand from cardata";

	try {
		pRst = conn.GetRecordSet(strSql);

		while (!pRst->adoEOF) {
			brandLst.Add(pRst->GetCollect("carbrand").bstrVal);

			pRst->MoveNext();
		}
	}
	catch (...) {
	}
}