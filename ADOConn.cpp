#include "stdafx.h"
#include "ADOConn.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
ADOConn::ADOConn()
{
	memset(error,0,1024);

}

ADOConn::~ADOConn()
{
}

//////////////////////////////////////////////////////////////////////
// CADOException Class
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CADOException::CADOException(char* pchMessage)
{
	m_strMessage = pchMessage;
	m_nError = GetLastError();
}

BOOL CADOException::GetErrorMessage(LPTSTR lpstrError, UINT nMaxError,PUINT pnHelpContext /*= NULL*/)
{
	char text[200];
	if(m_nError == 0)
	{
	   wsprintf(text, "%s error", (const char*) m_strMessage);
	}
	else
	{
	   wsprintf(text, "%s error #%d", (const char*) m_strMessage, m_nError);
	}
	strncpy(lpstrError, text, nMaxError - 1);
	return TRUE;
}

BOOL ADOConn::OnInitADOConn(LPCTSTR ConnStr)
{
	::CoInitialize(NULL);

	try
	{
	   // ����Connection����
	   m_pConnection.CreateInstance("ADODB.Connection");
	   // ���������ַ�����������BSTR�ͻ���_bstr_t����
	   _bstr_t strConnect = _bstr_t(ConnStr);//"Provider=SQLOLEDB; Server=127.0.0.1;Database=EventLogg; uid=event; pwd=event;";
	   m_pConnection->Open(strConnect,"","",adModeUnknown);
	   return TRUE;
	}
	// ��׽�쳣
	catch(_com_error e)
	{
		return false;
	   // ��ʾ������Ϣ
	    //TRACE(e.Description());
		//   sprintf(error,"�������ݿ�ʧ�� ���������ַ���\r\n��ǰ�����ַ���Ϊ:%s",ConnStr);
		//   throw new CADOException(error);
	    throw new CADOException("�������ݿ�ʧ��");
	}

}

//ִ��SQL��� ����update delete insert
BOOL ADOConn::ExecuteSQL(LPCTSTR lpszSQL)
{
	// _variant_t RecordsAffected;
	try
	{
	   // Connection�����Execute����:(_bstr_t CommandText,
	   // VARIANT * RecordsAffected, long Options )
	   // ����CommandText�������ִ���ͨ����SQL���
	   // ����RecordsAffected�ǲ�����ɺ���Ӱ�������,
	   // ����Options��ʾCommandText�����ͣ�adCmdText-�ı����adCmdTable-����
	   // adCmdProc-�洢���̣�adCmdUnknown-δ֪
	   m_pConnection->Execute(_bstr_t(lpszSQL),NULL,adCmdText);
	   return true;
	}
	catch(_com_error e)
	{
	   TRACE(e.Description());
		//   sprintf(error,"\tִ��SQL���ʧ��\r\n SQL���Ϊ:%s",lpszSQL);
		//   throw new CADOException(error);
	   throw new CADOException("ִ��SQL���ʧ��");
	}

}

//ִ��SELECT��䣬��ý���������������m_pRecordset��
_RecordsetPtr& ADOConn::GetRecordSet(LPCTSTR lpszSQL)
{
	try
	{
	   // ������¼������
	   m_pRecordset.CreateInstance(__uuidof(Recordset));
	   // ȡ�ñ��еļ�¼
	   m_pRecordset->Open(_bstr_t(lpszSQL),
		   m_pConnection.GetInterfacePtr(),
		   adOpenDynamic,adLockOptimistic,adCmdText);
	}
	// ��׽�쳣
	catch(_com_error e)
	{
	   // ��ʾ������Ϣ
	   TRACE(e.Description());
		//   sprintf(error,"\tִ��SELECT���ʧ��\r\nSELECT���Ϊ:%s",lpszSQL);
		//   throw new CADOException(error);
	   throw new CADOException("ִ��SELECT���ʧ��");
	}
	// ���ؼ�¼��
	return m_pRecordset;
}

//��ȡĳ���ֶε�ֵ
BOOL ADOConn::GetCollect(LPCTSTR Name,CString &lpDest)
{
	VARIANT vt;
	try
	{
	   vt = m_pRecordset->GetCollect(Name);

	   if(vt.vt!=VT_NULL)
		   lpDest=(LPCSTR)_bstr_t(vt);
	   else
		   lpDest="";
 
	}
	catch (_com_error e)
	{
	   TRACE(e.Description());
	   sprintf(error,"��ȡ�ֶ�:%sֵʧ��",Name);
	   throw new CADOException(error);
	}
	return TRUE;
}

// ��ȡĳ���������ֶε�ֵ
BOOL ADOConn::GetCollect(LPCTSTR Name, float &res)
{
	variant_t vt;

	try{
		vt = m_pRecordset->GetCollect(Name);

		if (vt.vt != VT_NULL) {
			vt.ChangeType(VT_R4);
			res = vt.fltVal;
		} else {
			return FALSE;
		}
	}catch (_com_error e) {
		TRACE (e.Description());
		throw new CADOException(error);
	}

	return TRUE;
}

//�ر����ݿ�
BOOL ADOConn::CloseADOConnection()
{
	try
	{
	   m_pConnection->Close();
	}
	catch (_com_error e)
	{
	   TRACE(e.Description());
	   sprintf(error,e.Description());
	   throw new CADOException("�ر����ݿ�ʧ��");
	}
	return TRUE;
}

//�ر�
BOOL ADOConn::CloseTable()
{
	try
	{
	   m_pRecordset->Close();
	}
	catch (_com_error e)
	{
	   TRACE(e.Description());
	   sprintf(error,e.Description());
	   throw new CADOException("�رձ�ʧ��");
	}
	return TRUE;
}

//��һ��
BOOL ADOConn::MoveNext()
{
	try
	{
	   m_pRecordset->MoveNext();
	}
	catch (_com_error e)
	{
	   TRACE(e.Description());
	   sprintf(error,e.Description());
	   throw new CADOException("�����������һ��ʧ��");
	}
	return TRUE;
}

//β
BOOL ADOConn::adoEOF()
{
	return m_pRecordset->adoEOF;
}

//ͷ
BOOL ADOConn::adoBOF()
{
	return m_pRecordset->BOF;
}

//����ʼ
BOOL ADOConn::BeginTrans()
{
	try
	{
	   m_pConnection->BeginTrans();
	}
	catch (_com_error e)
	{
	   TRACE(e.Description());
	   sprintf(error,e.Description());
	   throw new CADOException("����ʼʧ��");
	}
	return TRUE;
}

//�ݽ�����
BOOL ADOConn::CommitTrans()
{
	try
	{
	   m_pConnection->CommitTrans();
	}
	catch (_com_error e)
	{
	   TRACE(e.Description());
	   sprintf(error,e.Description());
	   throw new CADOException("�ݽ�����ʧ��");
	}
	return TRUE;
}

//����ع�
BOOL ADOConn::RollbackTrans()
{
	try
	{
	   m_pConnection->RollbackTrans();
	}
	catch (_com_error e)
	{
	   TRACE(e.Description());
	   sprintf(error,e.Description());
	   throw new CADOException("�ع�����ʧ��");
	}
	return TRUE;
}