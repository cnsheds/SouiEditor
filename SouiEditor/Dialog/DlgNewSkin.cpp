#include "stdafx.h"
#include "DlgNewSkin.h"
#include "CDebug.h"

extern SStringT g_CurDir;

namespace SOUI
{

	SDlgNewSkin::SDlgNewSkin(LPCTSTR pszXmlName):SHostDialog(pszXmlName)
	{

	}

	//TODO:��Ϣӳ��
	void SDlgNewSkin::OnCancel()
	{
		SHostDialog::OnCancel();
	}

	void SDlgNewSkin::OnOK()
	{
		int n = m_lbMain->GetCurSel();
		if (n >= 0)
		{
			m_lbMain->GetText(n, m_strSkinName);
		}
		else
		{
			CDebug::Debug(_T("��ѡ������һ��"));
			return;
		}
				
		SHostDialog::OnOK();
	}

	BOOL SDlgNewSkin::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
	{
		m_lbMain = FindChildByName2<SListBox>(L"NAME_UIDESIGNER_NEW_SKIN_LB");

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(g_CurDir + _T("Config\\SkinProperty.xml"));
		if (!result)
		{
			CDebug::Debug(_T("Config\\SkinProperty.xml����ʧ��"));
			return TRUE;
		}

		pugi::xml_node Node = doc.child(_T("root")).first_child();
		while (Node)
		{
			m_lbMain->AddString(Node.name());
			Node = Node.next_sibling();
		}

		return TRUE;
	}

}


