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

// QueryAllCarInfo() : 查询所有的车辆信息
// res : 保存查询结果并返回
void CCarOpt::QueryAllCarInfo(_RecordsetPtr &res)
{
	CString strSql;

	strSql = "select id,coding,number,carbrand,cartype,belongdept,\
				status=(case when carstatus=0 then '可用' else \
				case when carstatus=1 then '出车' else \
				case when carstatus=2 then '维修' else \
				case when carstatus=3 then '停用' else \
				case when carstatus=4 then '报废' else \
				case when carstatus=5 then '其它' end end end end end end),\
				driver,buytime,entertime,terminalnum  \
				from cardata";
	
	try {
		res = conn.GetRecordSet(strSql);
	}
	catch (...) {
	}
}

// SearchByText() : 对传递进来的文本进行模糊查询
// text : 将要进行搜索的文本
// pRst : 保存搜索结果
void CCarOpt::SearchByText(const CString &text, _RecordsetPtr &pRst)
{
	CString strSql;

	// 对数据库进行模糊查询的语句，查询的字段有：
	// number(车牌)、(车辆品牌)、(车辆类型)、(所属部门)、(驾驶员)、(终端号码)
	strSql = "select id,coding,number,carbrand,cartype,belongdept,\
				status=(case when carstatus=0 then '可用' else \
				case when carstatus=1 then '出车' else \
				case when carstatus=2 then '维修' else \
				case when carstatus=3 then '停用' else \
				case when carstatus=4 then '报废' else \
				case when carstatus=5 then '其它' end end end end end end),\
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

// GetCountByDate() : 根据登记日期查询同一天记录的条数，用于生成编码
// date : 将要进行查询的日期
// 返回值 : 返回指定登记日期的的同一天的记录条数，如果查询
//			出现异常返回 -1，如果没有同一天登记的记录返回 0
int CCarOpt::GetCountByDate(const COleDateTime &date)
{
	CString strSql;
	CString strDate;

	strDate = date.Format("%Y-%m-%d");

	// 查询记录条数的语句，查询结果保存在 res 中
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

// GetCountByNumber() : 查询与某车牌号对应的车辆数
//		用于判断是否有重复的车牌号
// number : 将要进行查询的车牌号
// 返回值 : 返回与指定车牌号对应的车辆数量，如果没有
//			对应的车辆返回 0，如果查询出现异常则返回 -1
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


// NewCarData() : 新增一条记录
// args : 保存新增记录的内容
BOOL CCarOpt::NewCarData(const CStringArray &args)
{
	CString strSql;
	int size;
	int index;

	strSql = "insert into cardata(coding,number,carbrand,cartype,belongdept,carstatus,\
				driver,buytime,entertime,terminalnum) values('";
	
	size = args.GetSize();

	// 循环获取传递进来的参数
	for (index = 0; index < (size - 1); ++index) {
		strSql += (args.GetAt(index) + "','");
	}

	strSql += args.GetAt(index) + "')"; // 添加最后一个参数

	try {
		conn.ExecuteSQL(strSql); // 执行 SQL 语句
	}
	catch (...) {
		return FALSE;
	}

	return TRUE;
}

// ModiCarData() : 修改车辆资料
// args : 将要更新的内容
BOOL CCarOpt::ModiCarData(const CStringArray &args)
{
	_ConnectionPtr pConn = conn.GetConnPtr();
	_RecordsetPtr pRst;
	CString strSql;

	strSql = "SELECT * FROM cardata where id in( \
				select id from sysobjects \
				where xtype='u' and [name]='设备管理')";
	
	try {
		// 获取表的所有字段
		pRst = pConn->Execute(_bstr_t(strSql),NULL,adCmdText);

		CString fieldStr;
		_variant_t var;
		UINT cols = pRst->GetFields()->Count; // 字段数
		strSql = "update cardata set ";
		// 遍历所有字段并结合成 SQL 语句
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

// DelCarData() : 删除车辆号对应的车辆资料
// number : 指定的编码
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

// GetIdByCoding() : 查询与指定编码对应的记录的ID并返回
// coding : 指定的编码
// 返回值 : 返回指定编码对应的ID，如果出现异常或没有该编码对应的
//			记录，则返回 -1
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

// GetNumber() : 查询车辆的ID和车牌号
// res : 保存查询结果并返回
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

// GetDriverById() : 查询与某 ID 号对应的车辆驾驶员
// id : 给定的查询 ID
// 返回值 : 如果查询成功返回查询结果
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

// GetCarType() : 获取车辆类型列表
// typeLst : 保存并返回车辆类型列表
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

// GetCarBrand() : 获取车辆商标列表
// brandLst : 保存并返回车辆商标列表
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