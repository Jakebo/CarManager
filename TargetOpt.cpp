// TargetOpt.cpp: implementation of the CTargetOpt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CarManage.h"
#include "TargetOpt.h"
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

CTargetOpt::CTargetOpt()
{

}

CTargetOpt::~CTargetOpt()
{

}

// QueryTarget() : 查询所有Target记录
// targetRecord : 保存返回的Target记录
void CTargetOpt::QueryTarget(std::vector<Target> &targetRecord)
{
	CString strQueryTarget; // 存储查询Target的语句
	int nIndex = 0;
	CString text;

	strSql = "select distinct cargroup=(select name from team where cargroup=team.id),\
				cargroupid=(select id from team where cargroup=team.id) \
				from target";

	_RecordsetPtr teamRst = conn.GetRecordSet(strSql);
	while (!teamRst->adoEOF) {
		Target target;

		target.cargroupName = teamRst->GetCollect("cargroup").bstrVal;
		targetRecord.push_back(target);

		strQueryTarget = "select id,name,longitude,latitude\
							from target \
							where cargroup=";
		text.Format("%d", teamRst->GetCollect("cargroupid").intVal);
		strQueryTarget += text;

		_RecordsetPtr &targetRst = conn.GetRecordSet(strQueryTarget);
		while (!targetRst->adoEOF) {
			TargetInfo targetInfo;

			// 获取Target的ID
			targetInfo.targetID = targetRst->GetCollect("id").lVal; 
			// 获取Target的名称
			if (VT_NULL != targetRst->GetCollect("name").vt) {
				text = targetRst->GetCollect("name").bstrVal;
			} else {
				text = "未命名";
			}
			targetInfo.targetName = text;
			// 获取Target的经度
			if (!conn.GetCollect("longitude", targetInfo.longitude)) {
				targetInfo.longitude = 0;
			}
			// 获取Target的纬度
			if (!conn.GetCollect("latitude", targetInfo.latitude)) {
				targetInfo.latitude = 0;
			}

			targetRecord[nIndex].targetLst.push_back(targetInfo);

			targetRst->MoveNext();
		}

		teamRst->MoveNext();
		++nIndex;
	}
}

// QueryTargetByUser() : 通过用户的权限来查询该用户所能管理的所有车辆
// userId : 用户的ID
// res : 保存并返回查询结果
void CTargetOpt::QueryTargetByUser(const int &userId,
								   std::vector<TargetInfo> &res)
{
	CString strSql;	

	strSql = "select target.id,target.name targetName,longitude,latitude,\
			    cargroup=(select name from team where id=target.cargroup) \
				from target";

	_RecordsetPtr pRst = conn.GetRecordSet(strSql);

	while (!pRst->adoEOF) {
		TargetInfo targetInfo;

		// 获取Target的ID
		targetInfo.targetID = pRst->GetCollect("id").lVal; 
		// 获取Target的名称
		if (VT_NULL != pRst->GetCollect("targetName").vt) {
			targetInfo.targetName = pRst->GetCollect("targetName").bstrVal;
		} else {
			targetInfo.targetName = "未命名";
		}
		// 获取Target的经度
		conn.GetCollect("longitude", targetInfo.longitude);
		// 获取Target的纬度
		conn.GetCollect("latitude", targetInfo.latitude);
		targetInfo.cargroup = pRst->GetCollect("cargroup").bstrVal;

		res.push_back(targetInfo);

		pRst->MoveNext();
	}
}

// QueryTargetPosition() : 通过某个Target的ID查询某个Target的位置
// targetID : Target 的ID
// longitude : Target 的经度
// latitude : Target 的纬度
void CTargetOpt::QueryTargetPosition(const int &targetID,
									 float &longitude,
									 float &latitude)
{
	CString tID;

	tID.Format("%d", targetID);
	strSql = "select longitude,latitude from target \
				where id=" + tID;

	_RecordsetPtr &pRst = conn.GetRecordSet(strSql);

	conn.GetCollect("longitude", longitude);
	conn.GetCollect("latitude", latitude);
}

// QueryTargetPosition() : 通过某个Target的ID查询某个Target的Coding
// targetID : Target 的 ID
// res : 返回Target 的 Coding
void CTargetOpt::QueryTargetCargroup(const int &targetID, CString &res)
{
	CString tID;

	tID.Format("%d", targetID);
	strSql = "select team.name from target,team \
				where target.id=" + tID + " and target.cargroup=team.coding";

	_RecordsetPtr &pRst = conn.GetRecordSet(strSql);

	res = pRst->GetCollect("name").bstrVal;
}

// QueryTargetLocus() : 通过target的ID查询该车辆在某个时间段运行的轨迹
// targetID : target 的 ID
// startTime : 起始时间
// endTime : 结束时间
// pRst : 保存结果的记录集
void CTargetOpt::QueryTargetLocus(const int &targetID, 
								  CString startTime,
								  CString endTime,
								  _RecordsetPtr &pRst)
{
	CString tID;

	tID.Format("%d", targetID);

	strSql = "select team.name cargroup,target.name targetName,track.time\
		,track.speed,track.longitude,track.latitude from team,target,track \
		where target.id=" + tID + " and \
			target.cargroup=team.coding and track.tid=target.id \
			and track.time>'" + startTime + "' and track.time<'" + endTime + 
			"' order by track.time";

	pRst = conn.GetRecordSet(strSql);
}

// QueryTargetInfo() : 通过target的ID查询该某车辆的详细信息
// res : 保存该车辆的详细信息
void CTargetOpt::QueryTargetInfo(const int &targetId,
								 CStringArray &res)
{
	CString id;
	float longitude;
	float latitude;

	id.Format("%d", targetId);

	CString strSql = "select number=(select number from cardata where terminalnum=target.coding),\
						carbrand=(select carbrand from cardata where terminalnum=target.coding),\
						target.coding targetCoding,target.name targetName,\
						driver=(select driver from cardata where terminalnum=target.coding),\
						driverphone=(select top 1 phone from driver,cardata where driver.name=cardata.driver),\
						adress,cargroup=(select name from team where id=cargroup),\
						speed,longitude,latitude \
						from target \
						where id=" + id;

	try {
		_RecordsetPtr &pRst = conn.GetRecordSet(strSql);

		if (!pRst->adoEOF) {
			int count = pRst->GetFields()->Count;
			// 循环获取前面所有字符串类型的字段值
			for (int i = 0; i < count - 3; ++i) {
				if (VT_NULL != pRst->GetCollect(long(i)).vt) { // 判断字段值是否为空
					res.Add(pRst->GetCollect(long(i)).bstrVal);
				} else {
					res.Add("");
				}
			}

			CString tmpStr;

			variant_t vt = pRst->GetCollect("speed");
			if (VT_NULL != vt.vt) {
				vt.ChangeType(VT_R4);
				float speed = vt.fltVal;
				tmpStr.Format("%lf", speed);
				res.Add(tmpStr);
			} else {
				res.Add("");
			}

			QueryTargetPosition(targetId, longitude, latitude);
			tmpStr.Format("%f", longitude);
			res.Add(tmpStr);
			tmpStr.Format("%f", latitude);
			res.Add(tmpStr);
		}
	} catch (...) {
	}
}


// QueryTargetInfo() : 查询所有车辆的信息
// pRst : 保存查询结果返回的记录集
void CTargetOpt::QueryTargetInfo(_RecordsetPtr &pRst)
{
	CString strSql ;

	strSql = "select target.coding,target.name targetName,adress,contract,\
				target.phoneno,[deny],online,\
				cargroup=(select name from team where cargroup=team.id),\
				usergroup,comment,[time]\
                from target";

	try {
		pRst = conn.GetRecordSet(strSql);
	}
	catch (...) {
	}
}

// QueryCodingById() : 通过 ID 来查询对应 TARGET 的编码
// id : 将要查询的 ID
// res : 保存并返回 TARGET 编码
void CTargetOpt::QueryCodingById(const int &id,
								 CString &res)
{
	CString strSql;
	CString tId;

	tId.Format("%d", id);

	strSql = "select coding from target \
				where id=" + tId;

	try {
		_RecordsetPtr &pRst = conn.GetRecordSet(strSql);
		
		if (!pRst->adoEOF) {
			res = pRst->GetCollect("coding").bstrVal;
		} else {
			res = _T("");
		}
	}
	catch (...) {
		res = _T("");
	}
}

// QueryIdByCoding() : 通过编码来查询对应终端的编码
// coding : 指定的终端编码
int CTargetOpt::QueryIdByCoding(const CString &coding)
{
	CString strSql;

	strSql = "select id from target \
				where coding=" + coding;

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

// GetTCByDate() : 获取某个日期添加的终端个数，用于生成唯一的终端编码
// date : 指定的某个日期
// 返回值 : 返回查询结果，即某个指定日期添加的终端个数
int CTargetOpt::GetTCByDate(const CTime &date)
{
	_RecordsetPtr pRst;
	CString strSql = "select count(coding) res from target where [time]='";
	strSql += date.Format("%Y-%m-%d") + "'";

	try {
		pRst = conn.GetRecordSet(strSql);
		if (!pRst->adoEOF) {
			return pRst->GetCollect("res").intVal;
		}
	}
	catch (...) {
	}

	return -1;
}

// AddTarget() : 添加一个新的终端
// content : 保存将要添加的终端资料内容
void CTargetOpt::AddTarget(const CStringArray &content)
{
	CString strSql;
	CString text;
	int size = content.GetSize();

	strSql = "insert into target(coding,name,contract,phoneno,\
				[deny],cargroup,usergroup,comment,password,time) \
				values(";

	for (int i = 0; i < (size - 1); ++i) {
		text = content.GetAt(i);
		if (text.GetLength() != 0) {
			strSql += "'" + text + "',";
		} else {
			strSql += "NULL,";
		}
	}

	strSql += "'" + content.GetAt(i) + "')";

	try {
		conn.ExecuteSQL(strSql);
	}
	catch (...) {
	}
}

// UpdateTarget() : 更新某终端记录
// coding : 指定更新的记录编码
// updateText : 已经组装好的文本
void CTargetOpt::UpdateTarget(const CString &coding,
								  const CString &updateText)
{
	CString strSql = "update target set ";

	strSql += updateText + " where coding='" + coding + "'";

	try {
		conn.ExecuteSQL(strSql);
	}
	catch (...)
	{
	}
}

// DelTarget() : 删除 CODING 对应的终端资料
// coding : 指定的 CODING
void CTargetOpt::DelTarget(const CString &coding)
{
	CString strSql = "delete from target where coding='";
	strSql += coding + "'";

	try {
		conn.ExecuteSQL(strSql);
	}
	catch (...) {
	}
}

// GetTeam() : 获取车队列表
// list : 保存返回的车队列表
void CTargetOpt::GetTeam(CStringArray &list)
{
	_RecordsetPtr pRst;
	CString strSql;

	strSql = "select distinct cargroup from target";

	try {
		pRst = conn.GetRecordSet(strSql);
		while (!pRst->adoEOF) {
			if (VT_NULL != pRst->GetCollect("cargroup").vt) {
				list.Add(pRst->GetCollect("cargroup").bstrVal);
			}
			pRst->MoveNext();
		}
	}
	catch (...) {
	}
}

// GetUnuseCoding() : 获取没用到的终端编码
// list : 保存并返回没用到的终端编码列表
void CTargetOpt::GetUnuseCoding(CStringArray &list)
{
	_RecordsetPtr pRst;
	CString strSql;

	strSql = "select distinct coding from target \
				where coding not in \
				(select terminalnum from cardata)";

	try {
		pRst = conn.GetRecordSet(strSql);
		while (!pRst->adoEOF) {
			list.Add(pRst->GetCollect("coding").bstrVal);
			pRst->MoveNext();
		}
	}
	catch (...) {
	}
}

// SearchByText() : 根据传递进来的文本进行模糊查询
//		进行模糊查询的字段有: 终端名、当前地址、所属车队、所属用户组
// searchText : 传递进来的文本
void CTargetOpt::SearchByText(const CString &text,
							  _RecordsetPtr &pRst)
{
	CString strSql;

	strSql = "select target.coding,target.name targetName,adress,contract,\
				target.phoneno,[deny],online,\
				cargroup=(select name from team where cargroup=team.id),\
				usergroup,comment,[time]\
                from target \
				where name like '%" + text + "%' or adress like '%" + text + 
				"%' or usergroup like '%" + text + "%' \
				or cargroup in (select id from team where name like '%" + text + "%')";

	try {
		pRst = conn.GetRecordSet(strSql);
	}
	catch (...)
	{
	}
}
