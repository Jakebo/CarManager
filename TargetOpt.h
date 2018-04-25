// TargetOpt.h: interface for the CTargetOpt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TARGETOPT_H__BA299838_60A7_4737_9623_F9255DA0D47D__INCLUDED_)
#define AFX_TARGETOPT_H__BA299838_60A7_4737_9623_F9255DA0D47D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <map>
using namespace std;

class TargetInfo {
public:
	int targetID;
	CString targetName;
	float longitude;
	float latitude;
	CString cargroup;
};

class Target {
public:
	CString cargroupName;
	std::vector<TargetInfo> targetLst;
};

class CTargetOpt  
{
public:
	CTargetOpt();
	virtual ~CTargetOpt();

	// 查询所有的Target记录
	// Target对象保存的是一个车队对应的所有车辆
	void QueryTarget(std::vector<Target> &targetRecord);
	// 通过用户的权限来查询该用户所能管理的所有车辆
	// 查询结果保存到参数 res 中
	void QueryTargetByUser(const int &userId,
		std::vector<TargetInfo> &res);
	// 通过某个Target的ID查询某个Target的位置
	void QueryTargetPosition(const int &targetID,
		float &longitude, 
		float &latitude);
	// 通过某个Target的ID查询某个Target的车队名
	void QueryTargetCargroup(const int &targetID, CString &res);
	// 通过Target的ID查询该车辆在某个时间段的轨迹
	void QueryTargetLocus(const int &targetID,
						  CString startTime,
						  CString endTime,
						  _RecordsetPtr &pRst);
	// 通过Target的ID查询该车辆的详细信息
	void QueryTargetInfo(const int &targetId, CStringArray &res);
	// 查询所有的车辆信息
	void QueryTargetInfo(_RecordsetPtr &pRst);
	// 通过ID来查询对应车辆的编码
	void QueryCodingById(const int &id, CString &res);
	// 通过终端编码来查询对应的ID
	int  QueryIdByCoding(const CString &coding);
	// 获取某日期添加的终端个数,用于生成终端编码
	int  GetTCByDate(const CTime &date);
	// 添加新的终端资料
	void AddTarget(const CStringArray &content);
	// 更新指定的终端记录
	void UpdateTarget(const CString &coding,
		const CString &updateText);
	// 删除一个终端资料
	void DelTarget(const CString &coding);
	// 获取车队列表
	void GetTeam(CStringArray &list);
	// 获取没有用到的终端编码
	void GetUnuseCoding(CStringArray &list);
	// 根据某文本来进行模糊查询
	void SearchByText(const CString &text, _RecordsetPtr &pRst);

private:
	CString strSql;

};

#endif // !defined(AFX_TARGETOPT_H__BA299838_60A7_4737_9623_F9255DA0D47D__INCLUDED_)
