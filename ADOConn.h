#if !defined(AFX_ADOCONN_H__2B491720_FA04_4800_B616_219E55ABEA46__INCLUDED_)
#define AFX_ADOCONN_H__2B491720_FA04_4800_B616_219E55ABEA46__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ADOConn : public CObject
{
private:
	//���һ��ָ��Connection�����ָ��:
	_ConnectionPtr m_pConnection;
	//���һ��ָ��Recordset�����ָ��:
	_RecordsetPtr m_pRecordset;
	char error[1024];
public:
	_ConnectionPtr& GetConnPtr()    {return m_pConnection;}
	_RecordsetPtr& GetRecoPtr()     {return m_pRecordset;}
	public:
	//����ع�
	BOOL RollbackTrans();
	//�ݽ� ����
	BOOL CommitTrans();
	//����ʼ
	BOOL BeginTrans();
	BOOL adoBOF();//ͷ
	BOOL adoEOF();//β
	BOOL MoveNext();//��һ��
	BOOL CloseTable();//�رձ�
	BOOL CloseADOConnection();//�ر�����
	BOOL GetCollect(LPCTSTR Name,CString &lpDest);//��ȡĳ���ֶε�ֵ
	BOOL GetCollect(LPCTSTR Name, float &res); // ��ȡĳ���������ֶε�ֵ
	//ִ��SQL��� ����update delete insert
	BOOL ExecuteSQL(LPCTSTR lpszSQL);
	//��ʼ���������ݿ�
	BOOL OnInitADOConn(LPCTSTR ConnStr);
	// ִ��select��� ��ȡ��¼��
	_RecordsetPtr& GetRecordSet(LPCTSTR lpszSQL);
	ADOConn();

	virtual ~ADOConn();
};

class CADOException : public CException
{
public:
	// Constructor
	CADOException(char* pchMessage);

public:
	~CADOException() {}
	CString m_strMessage;
	virtual BOOL GetErrorMessage(LPTSTR lpstrError,
								 UINT nMaxError,
								 PUINT pnHelpContext = NULL);
private:
	int m_nError;

};

#endif // !defined(AFX_ADOCONN_H__2B491720_FA04_4800_B616_219E55ABEA46__INCLUDED_)