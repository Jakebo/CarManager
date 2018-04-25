#if !defined(AFX_ADOCONN_H__2B491720_FA04_4800_B616_219E55ABEA46__INCLUDED_)
#define AFX_ADOCONN_H__2B491720_FA04_4800_B616_219E55ABEA46__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ADOConn : public CObject
{
private:
	//添加一个指向Connection对象的指针:
	_ConnectionPtr m_pConnection;
	//添加一个指向Recordset对象的指针:
	_RecordsetPtr m_pRecordset;
	char error[1024];
public:
	_ConnectionPtr& GetConnPtr()    {return m_pConnection;}
	_RecordsetPtr& GetRecoPtr()     {return m_pRecordset;}
	public:
	//事务回滚
	BOOL RollbackTrans();
	//递交 事务
	BOOL CommitTrans();
	//事务开始
	BOOL BeginTrans();
	BOOL adoBOF();//头
	BOOL adoEOF();//尾
	BOOL MoveNext();//下一个
	BOOL CloseTable();//关闭表
	BOOL CloseADOConnection();//关闭连接
	BOOL GetCollect(LPCTSTR Name,CString &lpDest);//获取某个字段的值
	BOOL GetCollect(LPCTSTR Name, float &res); // 获取某个浮点数字段的值
	//执行SQL语句 包含update delete insert
	BOOL ExecuteSQL(LPCTSTR lpszSQL);
	//初始化连接数据库
	BOOL OnInitADOConn(LPCTSTR ConnStr);
	// 执行select语句 获取记录集
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