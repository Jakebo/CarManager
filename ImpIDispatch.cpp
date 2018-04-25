/********************************************************************
	created:	2006/07/21
	created:	21:7:2006   22:15
	filename: 	D:\BeiJing\VC6\SmallTech\MFCHtml\ImpIDispatch.cpp
	file path:	D:\BeiJing\VC6\SmallTech\MFCHtml
	file base:	ImpIDispatch
	file ext:	cpp
	author:		������
	
	purpose:	����ʵ���Զ����ӿڣ�ʵ�ֺ���ҳ����
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


CString cszCB_IsOurCustomBrowser		= "CB_IsOurCustomBrowser"; // �Ƿ����Զ��������
CString cszCB_GetCarInfo				= "CB_GetCarInfo"; // ��ȡ������ϸ��Ϣ
CString cszCB_CreateTargetMarker		= "CB_CreateTargetMarker"; // �ڵ�ͼ�ϴ���������ע 
CString cszCB_CreateTraceMarker			= "CB_CreateTraceMarker"; // ��������ٵĳ�����ע��û����ʱ������

#define	DISPID_CB_IsOurCustomBrowser		1
#define DISPID_CB_GetCarInfo				2 // ��ȡ������ϸ��Ϣ
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
	
	// ��ȡ������ϸ��Ϣ
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

	// �ڵ�ͼ�ϴ����û�����������г����ı�ע
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

	// �����̬���ٴ��ڵı����ٳ����ı�ע��δ����������ú���������
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