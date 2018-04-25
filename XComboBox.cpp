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

// SetContent() : ���� COMBOBOX �����ݣ��������������
// content : ��Ҫ���õ����ݣ�INT Ϊ���ݣ�CSTRING Ϊ��ʾ���ı�
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

// SetContent() : ���� COMBOBOX ���ݣ��������������
// content : ��Ҫ���õ�����
void CXComboBox::SetContent(const CStringArray &content)
{
	int size = content.GetSize();

	for (int nIndex = 0; nIndex < size; ++nIndex) {
		this->AddString(content.GetAt(nIndex));
	}
}