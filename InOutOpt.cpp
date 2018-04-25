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

// QueryRecord() : 查询所有的派车记录
// pRst : 保存查询结果记录集并返回
void CInOutOpt::QueryRecord(_RecordsetPtr &pRst)
{
	CString strSql;

	strSql = "select outcar.coding,number,driver,driverphone,[user],\
				userphone,deptname=(case when dept is not null then \
				(select name from depts where depts.id=outcar.dept) else '' end),\
				usetime,backtime,startplace,\
				viaplace,destination,usereason,whoout,whopast,entertime,\
				backstatus=(case when isback=1 then '已收' else '未收' end),\
				whoback,comment \
				from outcar";

	try {
		pRst = conn.GetRecordSet(strSql);
	}
	catch (_com_error e){
	}
}

// QueryRecord() : 条件查询派车记录
// pRst : 保存查询结果记录集并返回
// searchText : 条件字符串
// select : 表示的是选择要查询的车辆集，0 表示查询所有车辆，
//			1 表示查询已收车辆，2 表示查询未收车辆
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
				backstatus=(case when isback=1 then '已收' else '未收' end),\
				whoback,comment \
				from outcar \
				where " + searchText;

	switch (select) {
	case 1:
		strSql += " and isback=1"; // 查询已收车辆
		break;
	case 2:
		strSql += " and (isback=0 or isback is null)"; // 查询未收车辆
		break;
	}

	try {
		pRst = conn.GetRecordSet(strSql.data());
	} 
	catch (_com_error e) {
	}
}

// QueryUnBackRecord() : 查询已收/未收车辆记录
// pRst : 保存查询结果记录集
void CInOutOpt::QueryRecord(_RecordsetPtr &pRst, BOOL isBack)
{
	std::string strSql;

	strSql = "select outcar.coding,number,driver,driverphone,[user],\
				userphone,deptname=(case when dept is not null then \
				(select name from depts where depts.id=outcar.dept) else '' end),\
				usetime,backtime,startplace,\
				viaplace,destination,usereason,whoout,whopast,entertime,\
				backstatus=(case when isback=1 then '已收' else '未收' end),\
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

// QueryRecordNumByDate() : 查询某天的派车数
// date : 某天的日期
// 返回值 : 返回查询结果，如果当天无派车数返回0，
//			如果查询发生异常返回 -1
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

// NewOutCarRecord() : 新增一条派车记录
// content : 记录的内容
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
				where xtype='u' and [name]='设备管理')";
	
	try {
		// 获取表的所有字段
		pRst = pConn->Execute(strSql.data(),NULL,adCmdText);

		UINT cols = pRst->GetFields()->Count; // 字段数
		strSql = "insert into outcar([";
		// 遍历所有字段并结合成 SQL 语句
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


// InComeCar() : 收车
// content:收车更新的内容
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
				where xtype='u' and [name]='设备管理')";
	
	try {
		// 获取表的所有字段
		pRst = pConn->Execute(strSql.data(),NULL,adCmdText);

		UINT cols = pRst->GetFields()->Count; // 字段数
		strSql = "update outcar set ";
		// 遍历所有字段并结合成 SQL 语句
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
 
// DetectLicit() : 检测某车牌号对应的车辆在某个时间段内是否可派
// number : 指定将要检测的车牌号
// usetime : 开始使用时间
// backtime : 估计返回时间
// 返回值 : 如果该车辆在该时间段内可派，返回 TRUE，否则返回 FALSE
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
			return TRUE; // 如果查询结果为空，表示可派
		} else {
			std::string tmU;
			std::string tmB;

			while (!pRst->adoEOF) {
				_variant_t date = pRst->GetCollect("usetime");
				tmU		 =  (COleDateTime(date).Format("%Y-%m-%d")).GetBuffer(0);
				date	 =  pRst->GetCollect("backtime");
				tmB		 =  (COleDateTime(date).Format("%Y-%m-%d")).GetBuffer(0);
				conflict += (tmU + "——" + tmB + ",\n");

				pRst->MoveNext();
			}

			return FALSE; // 否则表示不可派
		}
	}
	catch (_com_error e) {
	}

	return TRUE;
}