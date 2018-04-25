// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
#pragma warning(disable:4786)

#if !defined(AFX_STDAFX_H__44744BC0_8632_4DCF_9CBF_8E5132AD117A__INCLUDED_)
#define AFX_STDAFX_H__44744BC0_8632_4DCF_9CBF_8E5132AD117A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <afxtempl.h>
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#import "c:\program files\common files\system\ado\msado15.dll"\
no_namespace rename("EOF","adoEOF")

#endif // !defined(AFX_STDAFX_H__44744BC0_8632_4DCF_9CBF_8E5132AD117A__INCLUDED_)
