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

	void QueryAllCarInfo(_RecordsetPtr &res); // ��ѯ���г�������Ϣ
	// �Գ������Ͻ���ģ����ѯ
	void SearchByText(const CString &text, _RecordsetPtr &pRst); 
	int  GetCountByDate(const COleDateTime &date); // ��ѯĳ����¼��ĳ�����
	int	 GetCountByNumber(const CString &number); // ��ѯ��ĳ���ƺŶ�Ӧ�ĳ����� 
	BOOL NewCarData(const CStringArray &args); // ������������
	BOOL ModiCarData(const CStringArray &args); // �޸ĳ�������
	BOOL DelCarData(const CString &number); // ɾ����������
	int  GetIdByCoding(const CString &coding); // ��ѯ
	void GetNumber(std::map<int, CString> &res); // ��ѯ���г�����ID�ͳ��ƺ�
	void GetDriverById(const int &id, std::string &driver); //ͨ������ID��ѯ��ʻԱ
	void GetCarType(CStringArray &typeLst); // ��ȡ���������б�
	void GetCarBrand(CStringArray &brandLst); // ��ȡ�����̱��б�
};

#endif // !defined(AFX_CAROPT_H__0DA2B12A_FDEC_4C06_819E_C2BB780CABEE__INCLUDED_)
