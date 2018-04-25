/********************************************************************
	created:	2006/07/21
	created:	21:7:2006   22:15
	filename: 	D:\BeiJing\VC6\SmallTech\MFCHtml\ImpIDispatch.cpp
	file path:	D:\BeiJing\VC6\SmallTech\MFCHtml
	file base:	ImpIDispatch
	file ext:	cpp
	author:		万连文
	
	purpose:	重新实现自动化接口，实现和网页交互
*********************************************************************/

#include "stdafx.h"
#include "ImpIDispatch.h"

#include "CarManage.h"
#include "CarManageView.h"
#include "MapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CString cszCB_IsOurCustomBrowser		= "CB_IsOurCustomBrowser"; // 是否是自定义浏览器
CString cszCB_GetCarInfo				= "CB_GetCarInfo"; // 获取车辆详细信息
CString cszCB_CreateTargetMarker		= "CB_CreateTargetMarker"; // 在地图上创建车辆标注 
CString cszCB_CreateTraceMarker			= "CB_CreateTraceMarker"; // 如果被跟踪的车辆标注还没创建时被调用

#define	DISPID_CB_IsOurCustomBrowser		1
#define DISPID_CB_GetCarInfo				2 // 获取车辆详细信息
#define DISPID_CB_CreateTargetMarker		3
#define DISPID_CB_CreateTraceMarker			4 

CImpIDispatch::CImpIDispatch(void)
{
	m_cRef = 0;
}

CImpIDispatch::~CImpIDispatch(void)
{
	ASSERT(m_cRef==0);
}

STDMETHODIMP CImpIDispatch::QueryInterface(REFIID riid, void** ppv)
{
	*ppv = NULL;
	
	if(IID_IDispatch == riid)
	{
        *ppv = this;
	}
	
	if(NULL != *ppv)
    {
		((LPUNKNOWN)*ppv)->AddRef();
		return NOERROR;
	}
	
	return E_NOINTERFACE;
}


STDMETHODIMP_(ULONG) CImpIDispatch::AddRef(void)
{
	return ++m_cRef;
}

STDMETHODIMP_(ULONG) CImpIDispatch::Release(void)
{
	return --m_cRef;
}

STDMETHODIMP CImpIDispatch::GetTypeInfoCount(UINT* /*pctinfo*/)
{
	return E_NOTIMPL;
}

STDMETHODIMP CImpIDispatch::GetTypeInfo(
			/* [in] */ UINT /*iTInfo*/,
            /* [in] */ LCID /*lcid*/,
            /* [out] */ ITypeInfo** /*ppTInfo*/)
{
	return E_NOTIMPL;
}

STDMETHODIMP CImpIDispatch::GetIDsOfNames(
			/* [in] */ REFIID riid,
            /* [size_is][in] */ OLECHAR** rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID* rgDispId)
{
	HRESULT		hr		= NOERROR;
	UINT		i		= 0;
	CString		cszName	= rgszNames[i];

	for(i=0; i<cNames; ++i)
	{
		if(cszName == cszCB_IsOurCustomBrowser)
		{
			rgDispId[i] = DISPID_CB_IsOurCustomBrowser;
		} else if (cszName == cszCB_GetCarInfo) {
			rgDispId[i] = DISPID_CB_GetCarInfo;
		} else if (cszName == cszCB_CreateTargetMarker) {
			rgDispId[i] = DISPID_CB_CreateTargetMarker;
		} else if (cszName == cszCB_CreateTraceMarker) {
			rgDispId[i] = DISPID_CB_CreateTraceMarker;
		}else {
			// One or more are unknown so set the return code accordingly
			hr = ResultFromScode(DISP_E_UNKNOWNNAME);
			rgDispId[i] = DISPID_UNKNOWN;
		}
	}
	return hr;
}

STDMETHODIMP CImpIDispatch::Invoke(
			/* [in] */ DISPID dispIdMember,
			/* [in] */ REFIID /*riid*/,
			/* [in] */ LCID /*lcid*/,
			/* [in] */ WORD wFlags,
			/* [out][in] */ DISPPARAMS* pDispParams,
			/* [out] */ VARIANT* pVarResult,
			/* [out] */ EXCEPINFO* /*pExcepInfo*/,
			/* [out] */ UINT* puArgErr)
{
	CMapDlg*	pDlg	= mapDlg;
	if(dispIdMember == DISPID_CB_IsOurCustomBrowser)
	{
		if(wFlags & DISPATCH_PROPERTYGET)
		{
			if(pVarResult != NULL)
			{
				VariantInit(pVarResult);
				V_VT(pVarResult) = VT_BOOL;
				V_BOOL(pVarResult) = true;
			}
		}
		
		if(wFlags & DISPATCH_METHOD)
		{
			VariantInit(pVarResult);
			V_VT(pVarResult) = VT_BOOL;
			V_BOOL(pVarResult) = true;
		}
	}
	
	// 获取车辆详细信息
	if (dispIdMember == DISPID_CB_GetCarInfo)
	{
		if (wFlags & DISPATCH_PROPERTYGET) {
			if (pVarResult != NULL) {
				VariantInit (pVarResult);
				V_VT(pVarResult) = VT_BOOL;
				V_BOOL(pVarResult) = true;
			}
		}

		if (wFlags & DISPATCH_METHOD) {
			CString	strArg0	= pDispParams->rgvarg[0].bstrVal;
			pDlg->GetCarInfo(atoi((char*)LPCTSTR(strArg0)));
		}
	}

	// 在地图上创建用户所管理的所有车辆的标注
	if (dispIdMember == DISPID_CB_CreateTargetMarker)
	{
		if (wFlags & DISPATCH_PROPERTYGET) {
			if (pVarResult != NULL) {
				VariantInit(pVarResult);
				V_VT(pVarResult) = VT_BOOL;
				V_BOOL(pVarResult) = true;
			}
		}

		if (wFlags & DISPATCH_METHOD) {
			pDlg->CreateTargetMarker();
		}
	}

	// 如果动态跟踪窗口的被跟踪车辆的标注还未被创建，则该函数被调用
	if (dispIdMember == DISPID_CB_CreateTraceMarker)
	{
		if (wFlags & DISPATCH_PROPERTYGET) {
			if (pVarResult != NULL) {
				VariantInit(pVarResult);
				V_VT(pVarResult) = VT_BOOL;
				V_BOOL(pVarResult) = true;
			}
		}

		if (wFlags & DISPATCH_METHOD) {
			CString	strArg0	= pDispParams->rgvarg[0].bstrVal;
			if (!(strArg0.IsEmpty())) {
				pDlg->CreateTraceMarker(atoi((char*)LPCTSTR(strArg0)));
			}
		}
	}

	return S_OK;
}