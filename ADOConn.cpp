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
	   // 创建Connection对象
	   m_pConnection.CreateInstance("ADODB.Connection");
	   // 设置连接字符串，必须是BSTR型或者_bstr_t类型
	   _bstr_t strConnect = _bstr_t(ConnStr);//"Provider=SQLOLEDB; Server=127.0.0.1;Database=EventLogg; uid=event; pwd=event;";
	   m_pConnection->Open(strConnect,"","",adModeUnknown);
	   return TRUE;
	}
	// 捕捉异常
	catch(_com_error e)
	{
		return false;
	   // 显示错误信息
	    //TRACE(e.Description());
		//   sprintf(error,"连接数据库失败 请检查连接字符串\r\n当前连接字符串为:%s",ConnStr);
		//   throw new CADOException(error);
	    throw new CADOException("连接数据库失败");
	}

}

//执行SQL语句 包含update delete insert
BOOL ADOConn::ExecuteSQL(LPCTSTR lpszSQL)
{
	// _variant_t RecordsAffected;
	try
	{
	   // Connection对象的Execute方法:(_bstr_t CommandText,
	   // VARIANT * RecordsAffected, long Options )
	   // 其中CommandText是命令字串，通常是SQL命令。
	   // 参数RecordsAffected是操作完成后所影响的行数,
	   // 参数Options表示CommandText的类型：adCmdText-文本命令；adCmdTable-表名
	   // adCmdProc-存储过程；adCmdUnknown-未知
	   m_pConnection->Execute(_bstr_t(lpszSQL),NULL,adCmdText);
	   return true;
	}
	catch(_com_error e)
	{
	   TRACE(e.Description());
		//   sprintf(error,"\t执行SQL语句失败\r\n SQL语句为:%s",lpszSQL);
		//   throw new CADOException(error);
	   throw new CADOException("执行SQL语句失败");
	}

}

//执行SELECT语句，获得结果集，结果集放在m_pRecordset中
_RecordsetPtr& ADOConn::GetRecordSet(LPCTSTR lpszSQL)
{
	try
	{
	   // 创建记录集对象
	   m_pRecordset.CreateInstance(__uuidof(Recordset));
	   // 取得表中的记录
	   m_pRecordset->Open(_bstr_t(lpszSQL),
		   m_pConnection.GetInterfacePtr(),
		   adOpenDynamic,adLockOptimistic,adCmdText);
	}
	// 捕捉异常
	catch(_com_error e)
	{
	   // 显示错误信息
	   TRACE(e.Description());
		//   sprintf(error,"\t执行SELECT语句失败\r\nSELECT语句为:%s",lpszSQL);
		//   throw new CADOException(error);
	   throw new CADOException("执行SELECT语句失败");
	}
	// 返回记录集
	return m_pRecordset;
}

//获取某个字段的值
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
	   sprintf(error,"获取字段:%s值失败",Name);
	   throw new CADOException(error);
	}
	return TRUE;
}

// 获取某个浮点数字段的值
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

//关闭数据库
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
	   throw new CADOException("关闭数据库失败");
	}
	return TRUE;
}

//关表
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
	   throw new CADOException("关闭表失败");
	}
	return TRUE;
}

//下一个
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
	   throw new CADOException("结果集移向下一个失败");
	}
	return TRUE;
}

//尾
BOOL ADOConn::adoEOF()
{
	return m_pRecordset->adoEOF;
}

//头
BOOL ADOConn::adoBOF()
{
	return m_pRecordset->BOF;
}

//事务开始
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
	   throw new CADOException("事务开始失败");
	}
	return TRUE;
}

//递交事务
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
	   throw new CADOException("递交事务失败");
	}
	return TRUE;
}

//事务回滚
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
	   throw new CADOException("回滚事务失败");
	}
	return TRUE;
}