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

	void QueryRecord(_RecordsetPtr &pRst); // ��ѯ���е��ɳ���¼
	// ������ѯ�ɳ���¼
	void QueryRecord(_RecordsetPtr &pRst,
					 const std::string &searchText,
					 const UCHAR &select);
	// �����Ƿ����ղ�ѯ�ɳ���¼
	void QueryRecord(_RecordsetPtr &pRst, BOOL isBack);
	// ��ѯĳ����ɳ���
	int  QueryRecordNumByDate(const std::string &date);
	// ����һ���ɳ���¼
	void NewOutCarRecord(std::vector<std::string> &content);
	// �ճ�
	void InComeCar(std::vector<std::string> &content);
	// ���ĳ���ƺŶ�Ӧ�ĳ�����ĳ��ʱ����Ƿ����
	BOOL DetectLicit(const std::string &number, 
					 const std::string &usetime,
					 const std::string &backtime,
					 std::string &conflict);
};

#endif // !defined(AFX_INOUTOPT_H__AFB15197_02DA_4216_B2BB_A8E4812EBF45__INCLUDED_)
