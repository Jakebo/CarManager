// XComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "CarManage.h"
#include "XComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXComboBox

CXComboBox::CXComboBox()
{
}

CXComboBox::~CXComboBox()
{
}

BEGIN_MESSAGE_MAP(CXComboBox, CComboBox)
	//{{AFX_MSG_MAP(CXComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXComboBox message handlers

// SetContent() : 设置 COMBOBOX 的内容，不设置项的数据
// content : 将要设置的内容，INT 为数据，CSTRING 为显示的文本
void CXComboBox::SetContent(std::map<int, CString> &content)
{
	int nIndex;
	int size = content.size();
	std::map<int, CString>::iterator begin = content.begin();
	std::map<int, CString>::iterator end   = content.end();

	for (int i = 0; (i < size) && (begin != end); ++i, ++begin) {
		nIndex = this->AddString(begin->second);
		this->SetItemData(nIndex, begin->first);
	}
}

// SetContent() : 设置 COMBOBOX 内容，不设置项的数据
// content : 将要设置的内容
void CXComboBox::SetContent(const CStringArray &content)
{
	int size = content.GetSize();

	for (int nIndex = 0; nIndex < size; ++nIndex) {
		this->AddString(content.GetAt(nIndex));
	}
}