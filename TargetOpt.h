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

	// ��ѯ���е�Target��¼
	// Target���󱣴����һ�����Ӷ�Ӧ�����г���
	void QueryTarget(std::vector<Target> &targetRecord);
	// ͨ���û���Ȩ������ѯ���û����ܹ�������г���
	// ��ѯ������浽���� res ��
	void QueryTargetByUser(const int &userId,
		std::vector<TargetInfo> &res);
	// ͨ��ĳ��Target��ID��ѯĳ��Target��λ��
	void QueryTargetPosition(const int &targetID,
		float &longitude, 
		float &latitude);
	// ͨ��ĳ��Target��ID��ѯĳ��Target�ĳ�����
	void QueryTargetCargroup(const int &targetID, CString &res);
	// ͨ��Target��ID��ѯ�ó�����ĳ��ʱ��εĹ켣
	void QueryTargetLocus(const int &targetID,
						  CString startTime,
						  CString endTime,
						  _RecordsetPtr &pRst);
	// ͨ��Target��ID��ѯ�ó�������ϸ��Ϣ
	void QueryTargetInfo(const int &targetId, CStringArray &res);
	// ��ѯ���еĳ�����Ϣ
	void QueryTargetInfo(_RecordsetPtr &pRst);
	// ͨ��ID����ѯ��Ӧ�����ı���
	void QueryCodingById(const int &id, CString &res);
	// ͨ���ն˱�������ѯ��Ӧ��ID
	int  QueryIdByCoding(const CString &coding);
	// ��ȡĳ������ӵ��ն˸���,���������ն˱���
	int  GetTCByDate(const CTime &date);
	// ����µ��ն�����
	void AddTarget(const CStringArray &content);
	// ����ָ�����ն˼�¼
	void UpdateTarget(const CString &coding,
		const CString &updateText);
	// ɾ��һ���ն�����
	void DelTarget(const CString &coding);
	// ��ȡ�����б�
	void GetTeam(CStringArray &list);
	// ��ȡû���õ����ն˱���
	void GetUnuseCoding(CStringArray &list);
	// ����ĳ�ı�������ģ����ѯ
	void SearchByText(const CString &text, _RecordsetPtr &pRst);

private:
	CString strSql;

};

#endif // !defined(AFX_TARGETOPT_H__BA299838_60A7_4737_9623_F9255DA0D47D__INCLUDED_)
