// InOutOpt.h: interface for the CInOutOpt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INOUTOPT_H__AFB15197_02DA_4216_B2BB_A8E4812EBF45__INCLUDED_)
#define AFX_INOUTOPT_H__AFB15197_02DA_4216_B2BB_A8E4812EBF45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <vector>

class CInOutOpt  
{
public:
	CInOutOpt();
	virtual ~CInOutOpt();

	void QueryRecord(_RecordsetPtr &pRst); // 查询所有的派车记录
	// 条件查询派车记录
	void QueryRecord(_RecordsetPtr &pRst,
					 const std::string &searchText,
					 const UCHAR &select);
	// 根据是否已收查询派车记录
	void QueryRecord(_RecordsetPtr &pRst, BOOL isBack);
	// 查询某天的派车数
	int  QueryRecordNumByDate(const std::string &date);
	// 新增一条派车记录
	void NewOutCarRecord(std::vector<std::string> &content);
	// 收车
	void InComeCar(std::vector<std::string> &content);
	// 检测某车牌号对应的车辆在某个时间段是否可派
	BOOL DetectLicit(const std::string &number, 
					 const std::string &usetime,
					 const std::string &backtime,
					 std::string &conflict);
};

#endif // !defined(AFX_INOUTOPT_H__AFB15197_02DA_4216_B2BB_A8E4812EBF45__INCLUDED_)
