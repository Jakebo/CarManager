// TeamOpt.h: interface for the CTeamOpt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEAMOPT_H__427A0450_5629_4F41_9BEC_831644AA9D68__INCLUDED_)
#define AFX_TEAMOPT_H__427A0450_5629_4F41_9BEC_831644AA9D68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTeamOpt  
{
public:
	CTeamOpt();
	virtual ~CTeamOpt();

	// 获取所有的车队
	void GetTeam(std::map<int, CString> &teamLst);

};

#endif // !defined(AFX_TEAMOPT_H__427A0450_5629_4F41_9BEC_831644AA9D68__INCLUDED_)
