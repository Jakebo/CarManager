// CarOpt.h: interface for the CCarOpt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAROPT_H__0DA2B12A_FDEC_4C06_819E_C2BB780CABEE__INCLUDED_)
#define AFX_CAROPT_H__0DA2B12A_FDEC_4C06_819E_C2BB780CABEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

class CCarOpt  
{
public:
	CCarOpt();
	virtual ~CCarOpt();

	void QueryAllCarInfo(_RecordsetPtr &res); // 查询所有车辆的信息
	// 对车辆资料进行模糊查询
	void SearchByText(const CString &text, _RecordsetPtr &pRst); 
	int  GetCountByDate(const COleDateTime &date); // 查询某日期录入的车辆数
	int	 GetCountByNumber(const CString &number); // 查询与某车牌号对应的车辆数 
	BOOL NewCarData(const CStringArray &args); // 新增车辆资料
	BOOL ModiCarData(const CStringArray &args); // 修改车辆资料
	BOOL DelCarData(const CString &number); // 删除车辆资料
	int  GetIdByCoding(const CString &coding); // 查询
	void GetNumber(std::map<int, CString> &res); // 查询所有车辆的ID和车牌号
	void GetDriverById(const int &id, std::string &driver); //通过车辆ID查询驾驶员
	void GetCarType(CStringArray &typeLst); // 获取车辆类型列表
	void GetCarBrand(CStringArray &brandLst); // 获取车辆商标列表
};

#endif // !defined(AFX_CAROPT_H__0DA2B12A_FDEC_4C06_819E_C2BB780CABEE__INCLUDED_)
