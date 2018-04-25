// XCListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CarManage.h"
#include "XCListCtrl.h"
#include "ADOConn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXCListCtrl

CXCListCtrl::CXCListCtrl()
{
}

CXCListCtrl::~CXCListCtrl()
{
}


BEGIN_MESSAGE_MAP(CXCListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CXCListCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXCListCtrl message handlers
// GetCurSelContent() : ��ȡ��ǰѡ���е�����
// content : ����ѡ���е����ݲ�����
void CXCListCtrl::GetCurSelContent(CStringArray &content)
{
	UINT index = this->GetNextItem(-1, LVNI_SELECTED); // ��ȡ��ǰѡ�е���
	if (index == -1) {
		index = 0; // ���û��ѡ�м�¼��Ĭ��ѡ�е�һ��
	}

	CHeaderCtrl *m_pHeader = this->GetHeaderCtrl(); // ����б�ͷ
	UINT count = m_pHeader->GetItemCount(); // ��ȡ�б��ֶ���Ŀ
	
	for (UINT col = 0; col < count; ++col) {
		content.Add(this->GetItemText(index, col));
	}
}

// AddToLstTail() : �� content ������ӵ��б�β
// content : ��ӵ��б�β������
void CXCListCtrl::AddToLstTail(CStringArray &content)
{
	UINT index = this->GetItemCount(); // ��ȡ�б�������м�¼������
	UINT size = content.GetSize(); // ��ȡ��������

	this->InsertItem(index, content.GetAt(0)); // ���б��β�½�һ��
	for (UINT i = 1; i < size; ++i) {
		this->SetItemText(index, i, content.GetAt(i));
	}
}

// SetListContent() : ��ĳ��¼����������䵽�б���
// pRst : ĳ��¼��
void CXCListCtrl::SetListContent(_Recordset &pRst)
{
	CHeaderCtrl *m_pHeader = this->GetHeaderCtrl();
	UINT count  = m_pHeader->GetItemCount();
	int  fields = pRst.GetFields()->Count;
	int  column = count > fields ? fields : count;
	int  row = -1;
	int  col = -1;

	this->DeleteAllItems();
	while (!pRst.adoEOF) {
		this->InsertItem(++row, 
						ConvertVariantToString(pRst.GetCollect(long(0))));

		for (col = 1; col < column; ++col) {
			this->SetItemText(row,
							  col,
							  ConvertVariantToString(pRst.GetCollect(long(col))));
		}

		pRst.MoveNext();
	}
}

// ConvertVariantToString() : �� _variant_t ת��Ϊ String ����
// var : ��Ҫ����ת���� _variant_t ����
// ����ֵ : ����ת������ַ���
CString CXCListCtrl::ConvertVariantToString(const _variant_t &var)
{
	CString str;
	switch (var.vt) {
	case VT_BSTR:	// �ֶ�Ϊ�ַ�������
		str = var.bstrVal;
		break;

	case VT_I2:		// �ֶ�Ϊ������
		str.Format("%d", (int)var.iVal);
		break;

	case VT_I4:		// �ֶ�Ϊ������
		str.Format("%d", var.lVal);
		break;

	case VT_R4:		// �ֶ�Ϊ��������
		str.Format("%10.6f", (double)var.fltVal);
		break;

	case VT_R8:		// �ֶ�Ϊ˫���ȸ�����
		str.Format("%10.6f", var.dblVal);
		break;

	case VT_CY:		// �ֶ�ΪCY����
		str = COleDateTime(var).Format();
		break;

	case VT_DATE:	// �ֶ�Ϊ��������
		str = COleDateTime(var).Format();
		break;

	case VT_BOOL:
		str = (var.boolVal == 0) ? "FALSE" : "TRUE";
		break;

	default:
		break;
	}

	return str;
}


void CXCListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR; 
    *pResult  =  CDRF_DODEFAULT;

    switch (lplvcd->nmcd.dwDrawStage) 
    { 
    case CDDS_PREPAINT : 
        { 
            *pResult = CDRF_NOTIFYITEMDRAW; 
            return; 
        } 
    case CDDS_ITEMPREPAINT: 
        { 
            *pResult = CDRF_NOTIFYSUBITEMDRAW; 
            return; 
        } 
    case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
        { 
            int    nItem = static_cast<int>( lplvcd->nmcd.dwItemSpec );
            COLORREF clrNewTextColor, clrNewBkColor;
            clrNewTextColor = RGB(0, 0, 0);
            if( nItem % 2 == 0 )
            {
                clrNewBkColor = RGB(200, 216, 226); //ż���б���ɫΪ��ɫ
            }
            else
            {
                clrNewBkColor = RGB(223, 232, 238); //�����б���ɫΪ��ɫ
            }
            
            lplvcd->clrText = clrNewTextColor;
            lplvcd->clrTextBk = clrNewBkColor;
        }
    } 
}

// �����õıȽϺ���
static int CALLBACK
MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
     CString &lp1 = *((CString *)lParam1);
     CString &lp2 = *((CString *)lParam2);
     int &sort = *(int *)lParamSort;
     if (sort == 0)
     {
         return lp1.CompareNoCase(lp2);
     }
     else
     {
         return lp2.CompareNoCase(lp1);
     }
}

// OnColumnclick() : �����ͷʱ��Ӧ�˺���
void CXCListCtrl::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int Length = this->GetItemCount();
	CArray<CString,CString> ItemData;
	ItemData.SetSize(Length);

	for (int i = 0; i < Length; i++)
	{
		ItemData[i] = this->GetItemText(i,pNMLV->iSubItem);
		this->SetItemData(i,(DWORD)&ItemData[i]);//��������ؼ���
	}

	static int sort = 0;
	static int SubItem = 0;

	if (SubItem != pNMLV->iSubItem)
	{
		sort = 0;
		SubItem = pNMLV->iSubItem;
	}
	else
	{
		if (sort == 0)
		{
			sort = 1;
		}
		else
		{
			sort = 0;
		}
	}
	this->SortItems(MyCompareProc, (DWORD)&sort);//����

	*pResult = 0;
}
