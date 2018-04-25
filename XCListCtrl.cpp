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
// GetCurSelContent() : 获取当前选中行的内容
// content : 保存选中行的内容并返回
void CXCListCtrl::GetCurSelContent(CStringArray &content)
{
	UINT index = this->GetNextItem(-1, LVNI_SELECTED); // 获取当前选中的项
	if (index == -1) {
		index = 0; // 如果没有选中记录，默认选中第一条
	}

	CHeaderCtrl *m_pHeader = this->GetHeaderCtrl(); // 获得列表头
	UINT count = m_pHeader->GetItemCount(); // 获取列表字段数目
	
	for (UINT col = 0; col < count; ++col) {
		content.Add(this->GetItemText(index, col));
	}
}

// AddToLstTail() : 将 content 内容添加到列表尾
// content : 添加到列表尾的内容
void CXCListCtrl::AddToLstTail(CStringArray &content)
{
	UINT index = this->GetItemCount(); // 获取列表框中已有记录的条数
	UINT size = content.GetSize(); // 获取参数长度

	this->InsertItem(index, content.GetAt(0)); // 在列表结尾新建一行
	for (UINT i = 1; i < size; ++i) {
		this->SetItemText(index, i, content.GetAt(i));
	}
}

// SetListContent() : 将某记录集的内容填充到列表中
// pRst : 某记录集
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

// ConvertVariantToString() : 将 _variant_t 转换为 String 类型
// var : 将要进行转换的 _variant_t 变量
// 返回值 : 返回转换后的字符串
CString CXCListCtrl::ConvertVariantToString(const _variant_t &var)
{
	CString str;
	switch (var.vt) {
	case VT_BSTR:	// 字段为字符串类型
		str = var.bstrVal;
		break;

	case VT_I2:		// 字段为短整型
		str.Format("%d", (int)var.iVal);
		break;

	case VT_I4:		// 字段为长整型
		str.Format("%d", var.lVal);
		break;

	case VT_R4:		// 字段为单浮点型
		str.Format("%10.6f", (double)var.fltVal);
		break;

	case VT_R8:		// 字段为双精度浮点型
		str.Format("%10.6f", var.dblVal);
		break;

	case VT_CY:		// 字段为CY类型
		str = COleDateTime(var).Format();
		break;

	case VT_DATE:	// 字段为日期类型
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
                clrNewBkColor = RGB(200, 216, 226); //偶数行背景色为灰色
            }
            else
            {
                clrNewBkColor = RGB(223, 232, 238); //奇数行背景色为白色
            }
            
            lplvcd->clrText = clrNewTextColor;
            lplvcd->clrTextBk = clrNewBkColor;
        }
    } 
}

// 排序用的比较函数
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

// OnColumnclick() : 点击表头时响应此函数
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
		this->SetItemData(i,(DWORD)&ItemData[i]);//设置排序关键字
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
	this->SortItems(MyCompareProc, (DWORD)&sort);//排序

	*pResult = 0;
}
