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

// QueryTarget() : ��ѯ����Target��¼
// targetRecord : ���淵�ص�Target��¼
void CTargetOpt::QueryTarget(std::vector<Target> &targetRecord)
{
	CString strQueryTarget; // �洢��ѯTarget�����
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

			// ��ȡTarget��ID
			targetInfo.targetID = targetRst->GetCollect("id").lVal; 
			// ��ȡTarget������
			if (VT_NULL != targetRst->GetCollect("name").vt) {
				text = targetRst->GetCollect("name").bstrVal;
			} else {
				text = "δ����";
			}
			targetInfo.targetName = text;
			// ��ȡTarget�ľ���
			if (!conn.GetCollect("longitude", targetInfo.longitude)) {
				targetInfo.longitude = 0;
			}
			// ��ȡTarget��γ��
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

// QueryTargetByUser() : ͨ���û���Ȩ������ѯ���û����ܹ�������г���
// userId : �û���ID
// res : ���沢���ز�ѯ���
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

		// ��ȡTarget��ID
		targetInfo.targetID = pRst->GetCollect("id").lVal; 
		// ��ȡTarget������
		if (VT_NULL != pRst->GetCollect("targetName").vt) {
			targetInfo.targetName = pRst->GetCollect("targetName").bstrVal;
		} else {
			targetInfo.targetName = "δ����";
		}
		// ��ȡTarget�ľ���
		conn.GetCollect("longitude", targetInfo.longitude);
		// ��ȡTarget��γ��
		conn.GetCollect("latitude", targetInfo.latitude);
		targetInfo.cargroup = pRst->GetCollect("cargroup").bstrVal;

		res.push_back(targetInfo);

		pRst->MoveNext();
	}
}

// QueryTargetPosition() : ͨ��ĳ��Target��ID��ѯĳ��Target��λ��
// targetID : Target ��ID
// longitude : Target �ľ���
// latitude : Target ��γ��
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

// QueryTargetPosition() : ͨ��ĳ��Target��ID��ѯĳ��Target��Coding
// targetID : Target �� ID
// res : ����Target �� Coding
void CTargetOpt::QueryTargetCargroup(const int &targetID, CString &res)
{
	CString tID;

	tID.Format("%d", targetID);
	strSql = "select team.name from target,team \
				where target.id=" + tID + " and target.cargroup=team.coding";

	_RecordsetPtr &pRst = conn.GetRecordSet(strSql);

	res = pRst->GetCollect("name").bstrVal;
}

// QueryTargetLocus() : ͨ��target��ID��ѯ�ó�����ĳ��ʱ������еĹ켣
// targetID : target �� ID
// startTime : ��ʼʱ��
// endTime : ����ʱ��
// pRst : �������ļ�¼��
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

// QueryTargetInfo() : ͨ��target��ID��ѯ��ĳ��������ϸ��Ϣ
// res : ����ó�������ϸ��Ϣ
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
			// ѭ����ȡǰ�������ַ������͵��ֶ�ֵ
			for (int i = 0; i < count - 3; ++i) {
				if (VT_NULL != pRst->GetCollect(long(i)).vt) { // �ж��ֶ�ֵ�Ƿ�Ϊ��
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


// QueryTargetInfo() : ��ѯ���г�������Ϣ
// pRst : �����ѯ������صļ�¼��
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

// QueryCodingById() : ͨ�� ID ����ѯ��Ӧ TARGET �ı���
// id : ��Ҫ��ѯ�� ID
// res : ���沢���� TARGET ����
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

// QueryIdByCoding() : ͨ����������ѯ��Ӧ�ն˵ı���
// coding : ָ�����ն˱���
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

// GetTCByDate() : ��ȡĳ��������ӵ��ն˸�������������Ψһ���ն˱���
// date : ָ����ĳ������
// ����ֵ : ���ز�ѯ�������ĳ��ָ��������ӵ��ն˸���
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

// AddTarget() : ���һ���µ��ն�
// content : ���潫Ҫ��ӵ��ն���������
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

// UpdateTarget() : ����ĳ�ն˼�¼
// coding : ָ�����µļ�¼����
// updateText : �Ѿ���װ�õ��ı�
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

// DelTarget() : ɾ�� CODING ��Ӧ���ն�����
// coding : ָ���� CODING
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

// GetTeam() : ��ȡ�����б�
// list : ���淵�صĳ����б�
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

// GetUnuseCoding() : ��ȡû�õ����ն˱���
// list : ���沢����û�õ����ն˱����б�
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

// SearchByText() : ���ݴ��ݽ������ı�����ģ����ѯ
//		����ģ����ѯ���ֶ���: �ն�������ǰ��ַ���������ӡ������û���
// searchText : ���ݽ������ı�
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
